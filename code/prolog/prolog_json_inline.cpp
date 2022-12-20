
char * inline_json_resource = "import studio program json #machine := 'json' [json] #machine json := 'json' end := [[command]] .";

#include <stdlib.h>

class JSONReader {
public:
	FILE * fr;
	int act;
	AREA symbol;
	int SkipWhitespaces (void) {
		if (! fr) {act = -1; return -1;}
//		act = fgetc (fr);
		while (act <= ' ' && act >= 0) act = fgetc (fr);
		return act;
	};
	int SkipWhitespacesAndSeparators (void) {
		if (! fr) {act = -1; return -1;}
		while ((act <= ' ' && act >= 0) || act == ',') act = fgetc (fr);
		return act;
	};
	int GetSymbol (void) {
		if (! fr) return -1;
		SkipWhitespacesAndSeparators ();
		symbol [0] = '\0';
		if (strchr ("[]{}:", act) != 0) {symbol [0] = (char) act; symbol [1] = '\0'; act = fgetc (fr); return 1;}
		if (strchr ("-0123456789", act) != 0) {
			int ac = 0;
			while (strchr ("-+0123456789.eE", act) != 0) {ac = area_cat (symbol, ac, (char) act); act = fgetc (fr);}
			return 2;
		}
		if (act == '"') {
			act = fgetc (fr);
			int ac = 0;
			while (act >= 0 && act != '"') {
				if (act == '\\') {
					int ch = act = fgetc (fr);
					switch (ch) {
					case 'b': ch = 8; break;
					case 'f': ch = 12; break;
					case 'n': ch = 10; break;
					case 'r': ch = 13; break;
					case 't': ch = 9; break;
					default: break;
					}
					if (ch >= 0) ac = area_cat (symbol, ac, (char) ch);
				} else ac = area_cat (symbol, ac, (char) act);
				act = fgetc (fr);
			}
			if (act == '"') act = fgetc (fr);
			return 3;
		}
		if (act > ' ' && strchr ("[]{}:,\"", act) == 0) {
			int ac = 0; while (act > ' ' && strchr ("[]{}:,\"", act) == 0) {ac = area_cat (symbol, ac, (char) act); act = fgetc (fr);}
			return 4;
		}
		return 0;
	};
	JSONReader (char * file_name) {
		act = -1; symbol [0] = '\0';
		fr = fopen (file_name, "rb");
		if (! fr) return;
		act = fgetc (fr);
//		int ch; while ((ch = GetSymbol ()) > 0) printf ("%i => [%s] = %e <%c>\n", ch, symbol, double_symbol, act);
//		int ch; while ((ch = GetSymbol ()) > 0) printf ("%i => [%s] = %e\n", ch, symbol, double_symbol);
	};
	~ JSONReader (void) {if (fr) fclose (fr); fr = 0; printf ("File closed.\n");};
};

class json : public PrologNativeCode {
public:
	bool NeedSeparator;
	int SeparatorLevel;
	PrologRoot * root;
	FILE * tc;
	PrologAtom * ufo, * grlt, * equal, * assign;
	PrologAtom * TrueAtom, * FalseAtom, * NullAtom;
	int act;
	void DropSeparator (void) {
		if (NeedSeparator && tc) {fprintf (tc, ",\n"); int sl = SeparatorLevel; while (sl -- > 0 && tc) fprintf (tc, "	");}
		NeedSeparator = true;
	};
	void DropNull (void) {if (tc) fprintf (tc, "null");};
	void DropText (char * text) {
		if (! tc) return;
		char * ch = text;
		fprintf (tc, "\"");
		while (* ch > 0) {
			switch (* ch) {
			case '\\': fprintf (tc, "\\\\"); break;
			case '"': fprintf (tc, "\\\""); break;
			default: fprintf (tc, "%c", * ch); break;
			}
			ch ++;
		}
		fprintf (tc, "\"");
	};
	void DropAtom (PrologAtom * atom) {
		if (tc) {
			if (SeparatorLevel < 1 || atom == TrueAtom || atom == FalseAtom || atom == NullAtom) fprintf (tc, "%s", atom -> name ());
			else DropText (atom -> name ());
		}
	};
	void DropInteger (int value) {if (tc) fprintf (tc, "%i", value);};
	void DropDouble (double value) {if (tc) fprintf (tc, "%g", value);};
	void DropKeyValuePair (PrologElement * json) {
		if (! json -> isPair ()) return;
		PrologElement * key = json -> getLeft (), * value = json -> getRight ();
		DropJson (key); fprintf (tc, ": "); DropJson (value);
	};
	void DropPair (PrologElement * json) {
		if (! tc) return;
		PrologElement * ControlAtom = json -> getLeft ();
		if (! ControlAtom -> isAtom ()) return;
		if (ControlAtom -> getAtom () == ufo || ControlAtom -> getAtom () == grlt) {
			fprintf (tc, "[\n");
			NeedSeparator = false;
			json = json -> getRight ();
			SeparatorLevel ++;
			while (json -> isPair ()) {
				if (NeedSeparator) fprintf (tc, ",\n"); NeedSeparator = true;
				int sl = SeparatorLevel; while (sl -- > 0) fprintf (tc, "	");
				DropJson (json -> getLeft ());
				json = json -> getRight ();
			}
			SeparatorLevel --;
			if (NeedSeparator) fprintf (tc, "\n");
			int sl = SeparatorLevel; while (tc && sl -- > 0) fprintf (tc, "	");
			fprintf (tc, "]"); NeedSeparator = true;
		} else {
			fprintf (tc, "{\n");
			NeedSeparator = false;
			json = json -> getRight ();
			SeparatorLevel ++;
			while (json -> isPair ()) {
				if (NeedSeparator) fprintf (tc, ",\n"); NeedSeparator = true;
				int sl = SeparatorLevel; while (sl -- > 0) fprintf (tc, "	");
				DropKeyValuePair (json -> getLeft ());
				json = json -> getRight ();
			}
			SeparatorLevel --;
			if (NeedSeparator) fprintf (tc, "\n");
			int sl = SeparatorLevel; while (tc && sl -- > 0) fprintf (tc, "	");
			fprintf (tc, "}"); NeedSeparator = true;
		}
	};
	void DropJson (PrologElement * json) {
		if (json -> isEarth ()) DropNull ();
		else if (json -> isAtom ()) DropAtom (json -> getAtom ());
		else if (json -> isText ()) DropText (json -> getText ());
		else if (json -> isInteger ()) DropInteger (json -> getInteger ());
		else if (json -> isDouble ()) DropDouble (json -> getDouble ());
		else if (json -> isPair ()) DropPair (json);
	};
	PrologAtom * atomC (char * name) {
		PrologAtom * atom = root -> search (name);
		if (atom == 0) atom = root -> createAtom (name);
		return atom;
	};
	void ReadJSON (PrologElement * json, JSONReader * reader) {
		printf ("Symbol [%i] [%s].\n", act, reader -> symbol);
		switch (act) {
		case 1:
			switch (reader -> symbol [0]) {
			case '[':
				json -> setPair ();
				json -> getLeft () -> setAtom (ufo); json = json -> getRight ();
				act = reader -> GetSymbol ();
				while (act > 0 && (reader -> symbol [0] != ']' || act != 1)) {
//				while (act != 1 && reader -> symbol [0] != ']' && act > 0) {
					json -> setPair ();
					ReadJSON (json -> getLeft (), reader);
					json = json -> getRight ();
					act = reader -> GetSymbol ();
					printf ("ACT [%i] <%s>.\n", act, reader -> symbol);
				}
				break;
			case '{':
				json -> setPair ();
				json -> getLeft () -> setAtom (assign); json = json -> getRight ();
				act = reader -> GetSymbol ();
				while (act > 0 && (reader -> symbol [0] != '}' || act != 1)) {
					json -> setPair ();
					json -> getLeft () -> setPair ();
					ReadJSON (json -> getLeft () -> getLeft (), reader);
					act = reader -> GetSymbol ();
					if (act == 1 && reader -> symbol [0] == ':') {
						act = reader -> GetSymbol ();
						ReadJSON (json -> getLeft () -> getRight (), reader);
						json = json -> getRight ();
					} else act = reader -> GetSymbol ();
					act = reader -> GetSymbol ();
					printf ("ACT [%i] <%s>.\n", act, reader -> symbol);
				}
				break;
			default: break;
			}
			break;
		case 2:
			if (strchr (reader -> symbol, '.') != 0) json -> setDouble (atof (reader -> symbol));
			else json -> setInteger (atoi (reader -> symbol));
			break;
		case 3: case 4:
			if (strcmp (reader -> symbol, "true") == 0) json -> setAtom (TrueAtom);
			else if (strcmp (reader -> symbol, "false") == 0) json -> setAtom (FalseAtom);
			else if (strcmp (reader -> symbol, "null") == 0) json -> setEarth ();
			else json -> setAtom (atomC (reader -> symbol));
			break;
		default: break;
		}
	};
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (! parameters -> isPair ()) return false;
		PrologElement * json = parameters -> getLeft (); parameters = parameters -> getRight ();
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (json -> isVar ()) {
			if (! parameters -> isText ()) return false;
			JSONReader reader (parameters -> getText  ());
			act = reader . GetSymbol ();
			ReadJSON (json, & reader);
//			int ch = reader . GetSymbol ();
//			printf ("Symbol [%i] [%s].\n", ch, reader . symbol);			
			return true;
		} else {
			if (! parameters -> isText ()) return false;
			tc = fopen (parameters -> getText (), "wb");
			if (tc) {
				DropJson (json);
				fprintf (tc, "\n");
				fclose (tc);
			}
		}
		return true;
	};
	json (PrologRoot * root) {
		this -> root = root;
		NeedSeparator = false; SeparatorLevel = 0; tc = 0; act = -1;
		ufo = root -> search ("<=>");
		grlt = root -> search ("<>");
		equal = root -> search ("=");
		assign = root -> search (":=");
		TrueAtom = root -> search ("true");
		FalseAtom = root -> search ("false");
		NullAtom = root -> search ("null");
	};
};

class PrologJSONInlineServiceClass : public PrologServiceClass {
public:
	PrologRoot * root;
	void init (PrologRoot * root, PrologDirectory * directory) {this -> root = root;};
	PrologNativeCode * getNativeCode (char * name) {if (strcmp (name, "json") == 0) return new json (root); return 0;};
};

