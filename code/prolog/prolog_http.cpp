///////////////////////
// HTTP related code //
///////////////////////

#include <string.h>

#ifdef WIN32
#include <Winsock2.h>
#include <WS2tcpip.h>
#define close closesocket
#endif

#include "prolog_http.h"

#ifdef LINUX_OPERATING_SYSTEM
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

static void * get_in_addr (struct sockaddr * sa) {
	if (sa -> sa_family == AF_INET) return & (((struct sockaddr_in *) sa) -> sin_addr);
	return & (((struct sockaddr_in6 *) sa) -> sin6_addr);
}

class http_responder : public PrologNativeCode {
public:
	int fd;
	PrologRoot * root;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		AREA area;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isText ()) {char * cp = el -> getText (); send (fd, cp, strlen (cp), 0);}
			if (el -> isAtom ()) {char * cp = el -> getAtom () -> name (); send (fd, cp, strlen (cp), 0);}
			if (el -> isInteger ()) {sprintf (area, "%c", el -> getInteger ()); send (fd, area, 1, 0);}
			while (el -> isPair ()) {
				int length = root -> getValue (el -> getLeft (), area, 0);
				send (fd, area, length, 0);
				el = el -> getRight ();
			}
			parameters = parameters -> getRight ();
		}
		return true;
	}
	http_responder (int fd, PrologRoot * root) {this -> fd = fd; this -> root = root;}
};

class RequestAnalyser {
public:
	PrologHttpServiceClass * service;
	char * command;
	AREA area;
	char key [256];
	void get_word (void) {
		int ap = 0;
		while (* command > 32) ap = area_cat (area, ap, * command++);
		area_cat (area, ap, '\0');
	}
	void skip_whitespaces (void) {while (* command <= 32) command++;}
	void skip_line (void) {while (* command != 10 && * command > 0) command++; if (* command == 10) command++;}
	char extract_hex (char c1, char c2) {
		char ret;
		if ('0' <= c1 && c1 <= '9') ret = 16 * (c1 - '0');
		if ('A' <= c1 && c1 <= 'F') ret = 16 * (c1 - 'A' + 10);
		if ('a' <= c1 && c1 <= 'f') ret = 16 * (c1 - 'a' + 10);
		if ('0' <= c2 && c2 <= '9') ret += c2 - '0';
		if ('A' <= c2 && c2 <= 'F') ret += c2 - 'A' + 10;
		if ('a' <= c2 && c2 <= 'f') ret += c2 - 'a' + 10;
		return ret;
	}
	void copy_char (char * * to, char * * from) {
		char c1, c2;
		switch (* * from) {
		case '+': * (* to)++ = ' '; (* from)++; break;
		case '%': (* from)++; c1 = * (* from)++; c2 = * (* from)++; * (* to)++ = extract_hex (c1, c2); break;
		default: * (* to)++ = * (* from)++; break;
		}
	}
	PrologElement * interpret (void) {
		PrologAtom * atom = service -> root -> search (area);
		if (atom != 0) return service -> root -> atom (atom);
		double db = strtod (area, 0);
		int it = strtol (area, 0, 0);
		if (db != 0.0 && db != (int) it) return service -> root -> Double (db);
		if (it != 0) return service -> root -> integer (it);
		if (strcmp (area, "0.0") == 0) return service -> root -> Double (0.0);
		if (strcmp (area, "0") == 0) return service -> root -> integer (0);
		return service -> root -> text (area);
	}
	PrologElement * get_partial_request (void) {
		skip_whitespaces ();
		if (* command != '/') return 0;
		command++;
		int ap = 0; area [0] = '\0';
		char * cp = area;
		//while (* command != '/' && * command > 32) ap = area_cat (area, ap, * command++);
		while (* command != '/' && * command > 32) copy_char (& cp, & command);
		* cp = '\0';
		return interpret ();
	}
	void get_protocol (void) {
		skip_whitespaces ();
		int ap = 0; area [0] = '\0';
		while (* command > 32) ap = area_cat (area, ap, * command++);
		skip_line ();
	}
	bool get_header_line (void) {
		if (* command <= 32) {skip_line (); return false;}
		char * cp = key;
		while (* command > 32 && * command != ':') * cp++ = * command++;
		* cp = '\0';
		if (* command == ':') command++;
		skip_whitespaces ();
		int ap = 0; area [0] = '\0';
		while (* command >= 32) ap = area_cat (area, ap, * command++);
		skip_line ();
		return true;
	}
	bool get_param (void) {
		if (* command <= 32) {skip_line (); return false;}
		char * cp = key;
		while (* command > 32 && * command != '=') copy_char (& cp, & command);
		* cp = '\0';
		if (* command == '=') command++;
		cp = area;
		while (* command > 32 && * command != '&') copy_char (& cp, & command);
		* cp = '\0';
		if (* command == '&') command++;
		return true;
	}
	PrologAtom * get_method (void) {
		get_word ();
		if (strcmp (area, service -> get_atom -> name ()) == 0) return service -> get_atom;
		if (strcmp (area, service -> post_atom -> name ()) == 0) return service -> post_atom;
		if (strcmp (area, service -> put_atom -> name ()) == 0) return service -> put_atom;
		if (strcmp (area, service -> patch_atom -> name ()) == 0) return service -> patch_atom;
		if (strcmp (area, service -> delete_atom -> name ()) == 0) return service -> delete_atom;
		if (strcmp (area, service -> copy_atom -> name ()) == 0) return service -> copy_atom;
		if (strcmp (area, service -> head_atom -> name ()) == 0) return service -> head_atom;
		if (strcmp (area, service -> options_atom -> name ()) == 0) return service -> options_atom;
		if (strcmp (area, service -> link_atom -> name ()) == 0) return service -> link_atom;
		if (strcmp (area, service -> unlink_atom -> name ()) == 0) return service -> unlink_atom;
		if (strcmp (area, service -> purge_atom -> name ()) == 0) return service -> purge_atom;
		return 0;
	}
	RequestAnalyser (PrologHttpServiceClass * service, char * command) {this -> service = service; this -> command = command;}
};

class webserver : public PrologNativeCode {
public:
	PrologHttpServiceClass * service;
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * port = parameters -> getLeft ();
		if (! port -> isInteger ()) return false;
		parameters = parameters -> getRight ();
		if (! parameters -> isPair ()) return false;
		PrologElement * router = parameters -> getLeft ();
		if (! router -> isAtom ()) return false;
		char command [65536];
		sprintf (command, "%i", port -> getInteger ());
		struct addrinfo hints;
			memset (& hints, 0, sizeof (hints));
			hints . ai_family = AF_UNSPEC;
			hints . ai_socktype = SOCK_STREAM;
			hints . ai_flags = AI_PASSIVE;
		struct addrinfo * info;
		int status = getaddrinfo (0, command, & hints, & info);
		if (status != 0) {printf ("NETWORK ERROR [%s]\n", gai_strerror (status)); return false;}
		/*
		char ipstr [INET6_ADDRSTRLEN];
		printf ("RESULT:\n");
		printf ("	STATUS [%i]\n", status);
		printf ("	ERROR  [%s]\n", gai_strerror (status));
		for (addrinfo * ip = info; ip != 0; ip = ip -> ai_next) {
			void * address;
			char * v;
			if (ip -> ai_family == AF_INET) {
				struct sockaddr_in * ipv4 = (struct sockaddr_in *) ip -> ai_addr;
				address = & (ipv4 -> sin_addr);
				v = "IP:4";
			} else {
				struct sockaddr_in6 * ipv6 = (struct sockaddr_in6 *) ip -> ai_addr;
				address = & (ipv6 -> sin6_addr);
				v = "IP:6";
			}
			inet_ntop (ip -> ai_family, address, ipstr, sizeof (ipstr));
			printf ("	HOST %s [%s]\n", v, ipstr);
		}*/
		int sockfd;
#ifdef WINDOWS_OPERATING_SYSTEM
		char yes = '1';
#endif
#ifdef LINUX_OPERATING_SYSTEM
		int yes = 1;
#endif
		addrinfo * ip;
		for (ip = info; ip != 0; ip = ip -> ai_next) {
			if ((sockfd = socket (ip -> ai_family, ip -> ai_socktype, ip -> ai_protocol)) == -1) {
				perror ("server: socket\n");
				continue;
			}
			if (setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof (yes)) == -1) {
				perror ("setsockopt\n");
				return false;
			}
			if (bind (sockfd, ip -> ai_addr, ip -> ai_addrlen) == -1) {
				close (sockfd);
				perror ("server: bind\n");
				continue;
			}
			break;
		}
		if (ip == 0) {printf ("crap\n"); return false;}
		freeaddrinfo (info);
		// listening
		if (listen (sockfd, 10) == -1) {perror ("failed to listen\n"); return false;}
		socklen_t sin_size;
		struct sockaddr_storage their_addr;
		int new_fd;
		char s [INET6_ADDRSTRLEN];
		for (int sub = 0; sub < 12; sub++) {
//		while (1) {
			sin_size = sizeof (their_addr);
			new_fd = accept (sockfd, (struct sockaddr *) & their_addr, & sin_size);
			if (new_fd == -1) {printf ("accept crap\n"); return false;}
			inet_ntop (their_addr . ss_family, get_in_addr ((struct sockaddr *) & their_addr), s, sizeof (s));
//			if (! fork ()) {
				int read = recv (new_fd, command, 65536, 0);
				if (read < 0) {
					printf ("ERROR [%s]\n", strerror (read));
					close (new_fd);
					continue;
				}
				command [read] = '\0';

				PrologRoot * root = service -> root;
				PrologAtom * request = new PrologAtom ("HTTP-Request");
				PrologAtom * response = new PrologAtom ("HTTP-Response");
				response -> setMachine (new http_responder (new_fd, root));

RequestAnalyser analyser (service, command);
// FULL TEXT
PrologElement * clausa = root -> pair (root -> pair (root -> head (0), root -> pair (root -> atom (service -> full_text_atom), root -> pair (root -> text (command), root -> earth ()))), root -> earth ());
// METHOD
PrologAtom * method_atom = analyser . get_method ();
//clausa = root -> pair (root -> pair (root -> head (clausa), root -> pair (root -> atom (service -> method_atom), root -> pair (method_atom != 0 ? root -> atom (method_atom) : root -> text (analyser . area), root -> earth ()))), root -> earth ());
// URI
PrologElement * content = root -> pair (method_atom != 0 ? root -> atom (method_atom) : root -> text (analyser . area), root -> earth ());
PrologElement * cp = content -> getRight ();
PrologElement * partial = 0;
while ((partial = analyser . get_partial_request ()) != 0) {
	cp -> setPair (partial, root -> earth ());
	cp = cp -> getRight ();
}
clausa = root -> pair (root -> pair (root -> head (clausa), root -> pair (root -> atom (service -> route_atom), content)), root -> earth ());
// PROTOCOL
analyser . get_protocol ();
clausa = root -> pair (root -> pair (root -> head (clausa), root -> pair (root -> atom (service -> protocol_atom), root -> pair (root -> text (analyser . area), root -> earth ()))), root -> earth ());
// HEADER LINES
while (analyser . get_header_line ()) {
	content = root -> pair (root -> text (analyser . key), root -> pair (root -> text (analyser . area), root -> earth ()));
	clausa = root -> pair (root -> pair (root -> head (clausa), root -> pair (root -> atom (service -> header_atom), content)), root -> earth ());
}
// PARAMS
while (analyser . get_param ()) {
	content = root -> pair (root -> text (analyser . key), root -> pair (root -> text (analyser . area), root -> earth ()));
	clausa = root -> pair (root -> pair (root -> head (clausa), root -> pair (root -> atom (service -> param_atom), content)), root -> earth ());
}
				request -> firstClause = clausa;

				clausa = root -> pair (root -> atom (router -> getAtom ()), root -> pair (root -> atom (request), root -> pair (root -> atom (response), root -> earth ())));
				clausa = root -> pair (clausa, root -> earth ());
				clausa = root -> pair (root -> head (0), clausa);
				root -> resolution (clausa);
				delete clausa;
				//request -> removeAtom ();
				//response -> removeAtom ();
//				service -> full_text_atom -> removeAtom ();
//				close (new_fd);
//				exit (0);
//			}
			close (new_fd);
		}
		close (sockfd);
		return true;
	}
	webserver (PrologHttpServiceClass * service) {this -> service = service;}
};

void PrologHttpServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {
#ifdef WINDOWS_OPERATING_SYSTEM
	WORD version = MAKEWORD (2, 2);
	WSADATA wsadata;
	WSAStartup (version, & wsadata);
#endif
	this -> root = root;
	http_directory = directory;
	full_text_atom = route_atom = protocol_atom = header_atom = param_atom = 0;
	get_atom = post_atom = put_atom = patch_atom = delete_atom = copy_atom = 0;
	head_atom = options_atom = link_atom = unlink_atom = purge_atom = 0;
}

void PrologHttpServiceClass :: set_atoms (void) {
	if (full_text_atom != 0 || http_directory == 0) return;
	full_text_atom = http_directory -> searchAtom ("FULL_HEADER_TEXT");
	route_atom = http_directory -> searchAtom ("HTTP_URI");
	protocol_atom = http_directory -> searchAtom ("HTTP_PROTOCOL");
	header_atom = http_directory -> searchAtom ("HTTP_HEADER");
	param_atom = http_directory -> searchAtom ("param");
	get_atom = http_directory -> searchAtom ("GET");
	post_atom = http_directory -> searchAtom ("POST");
	put_atom = http_directory -> searchAtom ("PUT");
	patch_atom = http_directory -> searchAtom ("PATCH");
	delete_atom = http_directory -> searchAtom ("DELETE");
	copy_atom = http_directory -> searchAtom ("COPY");
	head_atom = http_directory -> searchAtom ("HEAD");
	options_atom = http_directory -> searchAtom ("OPTIONS");
	link_atom = http_directory -> searchAtom ("LINK");
	unlink_atom = http_directory -> searchAtom ("UNLINK");
	purge_atom = http_directory -> searchAtom ("PURGE");
}

PrologNativeCode * PrologHttpServiceClass :: getNativeCode (char * name) {
	set_atoms ();
	if (strcmp (name, "webserver") == 0) return new webserver (this);
	return 0;
}

