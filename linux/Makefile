
SHELL = /bin/sh

#### Start of system configuration section. ####

srcdir = .
topdir = /usr/lib/ruby/1.8/i486-linux
hdrdir = $(topdir)
VPATH = $(srcdir):$(topdir):$(hdrdir)
exec_prefix = $(prefix)
prefix = $(DESTDIR)/usr
sharedstatedir = $(prefix)/com
mandir = $(prefix)/share/man
psdir = $(docdir)
oldincludedir = $(DESTDIR)/usr/include
localedir = $(datarootdir)/locale
bindir = $(exec_prefix)/bin
libexecdir = $(prefix)/lib/ruby1.8
sitedir = $(DESTDIR)/usr/local/lib/site_ruby
htmldir = $(docdir)
vendorarchdir = $(vendorlibdir)/$(sitearch)
includedir = $(prefix)/include
infodir = $(prefix)/share/info
vendorlibdir = $(vendordir)/$(ruby_version)
sysconfdir = $(DESTDIR)/etc
libdir = $(exec_prefix)/lib
sbindir = $(exec_prefix)/sbin
rubylibdir = $(libdir)/ruby/$(ruby_version)
docdir = $(datarootdir)/doc/$(PACKAGE)
dvidir = $(docdir)
vendordir = $(libdir)/ruby/vendor_ruby
datarootdir = $(prefix)/share
pdfdir = $(docdir)
archdir = $(rubylibdir)/$(arch)
sitearchdir = $(sitelibdir)/$(sitearch)
datadir = $(datarootdir)
localstatedir = $(DESTDIR)/var
sitelibdir = $(sitedir)/$(ruby_version)

# MODIFICATION Copmilator changed to g++
CC = g++
LIBRUBY = $(LIBRUBY_SO)
LIBRUBY_A = lib$(RUBY_SO_NAME)-static.a
LIBRUBYARG_SHARED = -l$(RUBY_SO_NAME)
LIBRUBYARG_STATIC = -l$(RUBY_SO_NAME)-static

RUBY_EXTCONF_H = 
CFLAGS   =  -fPIC -fno-strict-aliasing -g -g -O2  -fPIC $(cflags) 
INCFLAGS = -I. -I$(topdir) -I$(hdrdir) -I$(srcdir)
DEFS     = -D_FILE_OFFSET_BITS=64
CPPFLAGS =   $(DEFS) $(cppflags)
CXXFLAGS = $(CFLAGS) 
ldflags  = -L. -Wl,-Bsymbolic-functions -rdynamic -Wl,-export-dynamic
dldflags = 
archflag = 
DLDFLAGS = $(ldflags) $(dldflags) $(archflag)
LDSHARED = $(CC) -shared
AR = ar
EXEEXT = 

RUBY_INSTALL_NAME = ruby1.8
RUBY_SO_NAME = ruby1.8
arch = i486-linux
sitearch = i486-linux
ruby_version = 1.8
ruby = /usr/bin/ruby1.8
RUBY = $(ruby)
RM = rm -f
MAKEDIRS = mkdir -p
INSTALL = /usr/bin/install -c
INSTALL_PROG = $(INSTALL) -m 0755
INSTALL_DATA = $(INSTALL) -m 644
COPY = cp

#### End of system configuration section. ####

# MODIFICATION cp (Code Path) = location of the source code ../code/prolog/*.*
pc = ../code/prolog/

preload = 

libpath = . $(libdir)
LIBPATH =  -L. -L$(libdir)
DEFFILE = 

CLEANFILES = mkmf.log
DISTCLEANFILES = 

extout = 
extout_prefix = 
target_prefix = 
LOCAL_LIBS = 
LIBS = $(LIBRUBYARG_SHARED)  -lpthread -lrt -ldl -lcrypt -lm `mysql_config --libs` -lc -lreadline
# MODIFICATION Source code locations added in the beginning of every source code
SRCS = $(pc)object_tracker.cpp $(pc)prolog_midi_command.cpp $(pc)prolog_reader.cpp $(pc)midi_stream.cpp $(pc)prolog_directory.cpp $(pc)prolog_service_class.cpp $(pc)prolog_dll_loader.cpp $(pc)linux_midi.cpp $(pc)prolog_query.cpp $(pc)hercs_gem.cpp $(pc)prolog_loader.cpp $(pc)prolog_match.cpp $(pc)prolog_element.cpp $(pc)prolog_transport.cpp $(pc)prolog_command.cpp $(pc)midi_command_prompt.cpp $(pc)prolog_native_code.cpp $(pc)prolog_neural.cpp $(pc)encoder.cpp $(pc)prolog_root.cpp $(pc)prolog_resolution.cpp $(pc)prolog_variable.cpp $(pc)prolog_resources.cpp $(pc)prolog_linux_console.cpp $(pc)prolog_studio.cpp $(pc)prolog_conductor.cpp $(pc)prolog_midi.cpp $(pc)prolog_http.cpp $(pc)prolog_atom.cpp
OBJS = object_tracker.o prolog_midi_command.o prolog_reader.o midi_stream.o prolog_directory.o prolog_service_class.o prolog_dll_loader.o linux_midi.o prolog_query.o hercs_gem.o prolog_loader.o prolog_match.o prolog_element.o prolog_transport.o prolog_command.o midi_command_prompt.o prolog_native_code.o prolog_neural.o encoder.o prolog_root.o prolog_resolution.o prolog_variable.o prolog_linux_console.o prolog_studio.o prolog_conductor.o prolog_midi.o prolog_http.o prolog_neural.o prolog_atom.o
TARGET = HERCs
DLLIB = $(TARGET).so
EXTSTATIC = 
STATIC_LIB = 

BINDIR        = $(bindir)
RUBYCOMMONDIR = $(sitedir)$(target_prefix)
RUBYLIBDIR    = $(sitelibdir)$(target_prefix)
RUBYARCHDIR   = $(sitearchdir)$(target_prefix)

TARGET_SO     = $(DLLIB)
CLEANLIBS     = $(TARGET).so $(TARGET).il? $(TARGET).tds $(TARGET).map
CLEANOBJS     = *.o *.a *.s[ol] *.pdb *.exp *.bak

all:		$(DLLIB)
static:		$(STATIC_LIB)

clean:
		@-$(RM) $(CLEANLIBS) $(CLEANOBJS) $(CLEANFILES)

distclean:	clean
		@-$(RM) Makefile $(RUBY_EXTCONF_H) conftest.* mkmf.log
		@-$(RM) core ruby$(EXEEXT) *~ $(DISTCLEANFILES)

realclean:	distclean
install: install-so install-rb

install-so: $(RUBYARCHDIR)
install-so: $(RUBYARCHDIR)/$(DLLIB)
$(RUBYARCHDIR)/$(DLLIB): $(DLLIB)
	$(INSTALL_PROG) $(DLLIB) $(RUBYARCHDIR)
install-rb: pre-install-rb install-rb-default
install-rb-default: pre-install-rb-default
pre-install-rb: Makefile
pre-install-rb-default: Makefile
$(RUBYARCHDIR):
	$(MAKEDIRS) $@

site-install: site-install-so site-install-rb
site-install-so: install-so
site-install-rb: install-rb

.SUFFIXES: .c .m .cc .cxx .cpp .C .o

.cc.o:
	$(CXX) $(INCFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $<

.cxx.o:
	$(CXX) $(INCFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $<

.cpp.o:
	$(CXX) $(INCFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $<

.C.o:
	$(CXX) $(INCFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $<

.c.o:
	$(CC) $(INCFLAGS) $(CPPFLAGS) $(CFLAGS) -c $<

$(DLLIB): $(OBJS) Makefile
	@-$(RM) $@
	$(LDSHARED) -o $@ $(OBJS) $(LIBPATH) $(DLDFLAGS) $(LOCAL_LIBS) $(LIBS)



#$(OBJS): ruby.h defines.h






# MODIFICATION
objects = \
	$(pi)prolog_dll_loader.o\
	$(pi)midi_stream.o $(pi)object_tracker.o $(pi)prolog_atom.o $(pi)prolog_command.o $(pi)prolog_directory.o \
	$(pi)prolog_element.o $(pi)prolog_loader.o $(pi)prolog_match.o $(pi)prolog_midi_command.o \
	$(pi)prolog_native_code.o $(pi)prolog_query.o $(pi)prolog_reader.o $(pi)prolog_resolution.o \
	$(pi)prolog_root.o $(pi)prolog_service_class.o \
	$(pi)prolog_studio.o $(pi)prolog_conductor.o $(pi)prolog_midi.o $(pi)prolog_neural.o $(pi)prolog_http.o \
	$(pi)prolog_transport.o \
	$(pi)prolog_variable.o \
	$(pi)prolog_linux_console.o \
	$(pi)prolog_xml.o $(pi)tinyxml.o \
	$(pi)prolog_distribution.o $(pi)prolog_control.o $(pi)joystick_linux.o

prc_resources = studio_prc.o conductor_prc.o midi_prc.o http_prc.o store_prc.o f1_prc.o help_prc.o record_prc.o\
	neural_prc.o keyboard_prc.o sql_prc.o test_prc.o xml_prc.o distribution_prc.o control_prc.o

hercs_gem.o: $(pc)hercs_gem.cpp
	g++ -Wno-write-strings -I. $(INCFLAGS) $(pc)hercs_gem.cpp -c -o hercs_gem.o
midi_stream.o: $(pc)midi_stream.h $(pc)midi_stream.cpp
	g++ -Wno-write-strings -std=c++11 -I. $(pc)midi_stream.cpp -c -o midi_stream.o
object_tracker.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)object_tracker.h $(pc)object_tracker.cpp
	g++ -Wno-write-strings -I. $(pc)object_tracker.cpp -c -o object_tracker.o
prolog_atom.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)object_tracker.h $(pc)prolog_atom.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_atom.cpp -c -o prolog_atom.o
prolog_command.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_command.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_command.cpp -c -o prolog_command.o
prolog_directory.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_directory.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_directory.cpp -c -o prolog_directory.o
prolog_element.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_element.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_element.cpp -c -o prolog_element.o
prolog_loader.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_loader.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_loader.cpp -c -o prolog_loader.o
prolog_match.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_match.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_match.cpp -c -o prolog_match.o
prolog_midi_command.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_midi_command.h $(pc)prolog_midi_command.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_midi_command.cpp -c -o prolog_midi_command.o
prolog_native_code.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_native_code.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_native_code.cpp -c -o prolog_native_code.o
prolog_query.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_query.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_query.cpp -c -o prolog_query.o
prolog_reader.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_reader.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_reader.cpp -c -o prolog_reader.o
prolog_resolution.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_resolution.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_resolution.cpp -c -o prolog_resolution.o
prolog_root.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_root.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_root.cpp -c -o prolog_root.o
prolog_service_class.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_service_class.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_service_class.cpp -c -o prolog_service_class.o
prolog_studio.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_studio.h $(pc)prolog_studio.cpp
	g++ -Wno-write-strings -std=c++11 -I. $(pc)prolog_studio.cpp -Wno-write-strings -c -o prolog_studio.o
prolog_conductor.o: $(pc)prolog.h $(pc)prolog_conductor.h $(pc)prolog_conductor.cpp $(pc)prolog_transport.h
	g++ -Wno-write-strings -I. $(pc)prolog_conductor.cpp -c -o prolog_conductor.o
prolog_midi.o: $(pc)prolog.h $(pc)prolog_midi.h $(pc)prolog_midi.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_midi.cpp -c -o prolog_midi.o
prolog_http.o: $(pc)prolog.h $(pc)prolog_http.h $(pc)prolog_http.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_http.cpp -c -o prolog_http.o
prolog_transport.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_transport.h $(pc)prolog_transport.cpp
	g++ -Wno-write-strings -std=c++11 -I. $(pc)prolog_transport.cpp -c -o prolog_transport.o
prolog_variable.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_variable.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_variable.cpp -c -o prolog_variable.o
prolog_neural.o: $(pc)prolog_neural.h $(pc)prolog_neural.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_neural.cpp -c -o prolog_neural.o
prolog_xml.o: $(pc)prolog_xml.h $(pc)prolog_xml.cpp $(pc)prolog.h
	g++ -Wno-write-strings -I. $(pc)prolog_xml.cpp -c -o prolog_xml.o
tinyxml.o: $(pc)../tinyxml2/tinyxml2.cpp $(pc)../tinyxml2/tinyxml2.h
	g++ -Wno-write-strings $(pc)../tinyxml2/tinyxml2.cpp -c -o tinyxml.o
prolog_distribution.o: $(pc)prolog_distribution.h $(pc)prolog_distribution.cpp $(pc)prolog.h
	g++ -Wno-write-strings -std=c++11 -I. $(pc)prolog_distribution.cpp -c -o prolog_distribution.o
prolog_control.o: $(pc)prolog_control.h $(pc)prolog_control.cpp $(pc)prolog.h
	g++ -Wno-write-strings -I. $(pc)prolog_control.cpp -c -o prolog_control.o
joystick_linux.o: $(pc)joystick_linux.h $(pc)joystick_linux.cpp
	g++ -Wno-write-strings -I. $(pc)joystick_linux.cpp -c -o joystick_linux.o
prolog_mysql.o: $(pc)prolog_mysql.cpp
	g++ -Wno-write-strings `mysql_config --cflags` -I. $(pc)prolog_mysql.cpp -c -o prolog_mysql.o

prolog_dll_loader.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_windows_console.h $(pc)prolog_dll_loader.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_dll_loader.cpp -c -o prolog_dll_loader.o
prolog_windows_console.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_windows_console.h
	g++ -Wno-write-strings -I. $(pc)prolog_windows_console.cpp -c -o prolog_windows_console.o
prolog_linux_console.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_linux_console.h $(pc)prolog_linux_console.cpp
	g++ -Wno-write-strings -I. $(pc)prolog_linux_console.cpp -c -o prolog_linux_console.o

encoder.o: $(pc)encoder.h $(pc)encoder.cpp
	g++ -Wno-write-strings -I. $(pc)encoder.cpp -c -o encoder.o

linux_midi.o: $(pc)prolog.h $(pc)linux_midi.h $(pc)linux_midi.cpp
	g++ -Wno-write-strings -I. $(pc)linux_midi.cpp -c -o linux_midi.o

midi_command_prompt.o: $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)midi_command_prompt.cpp $(pc)midi_command_prompt.h
	g++ -Wno-write-strings -I. $(pc)midi_command_prompt.cpp -c -o midi_command_prompt.o

prc.o: $(pc)linux_midi.h $(pc)prolog.h $(pc)midi_stream.h $(pc)encoder.h $(pc)prolog_hercs_console.h \
	$(pc)prolog_midi_command.h $(pc)prolog_windows_console.h \
	$(pc)prolog_studio.h $(pc)prolog_conductor.h $(pc)prolog_midi.h $(pc)prolog_http.h $(pc)midi_command_prompt.h \
	$(pc)prc.cpp $(pc)prolog_neural.h
	g++ -Wno-write-strings -I. -I$(pc) $(pc)prc.cpp -c -o prc.o -lreadline


/usr/lib/prolog_mysql.so: prolog_mysql.o
	g++ -lpthread -fPIC -ldl -Wl,--export-dynamic prolog_mysql.o `mysql_config --libs` -o /usr/lib/prolog_mysql.so -shared


~/bin/mysql.prc: $(pc)mysql.prc
	cp $(pc)mysql.prc ~/bin/mysql.prc

mysql: /usr/lib/prolog_mysql.so ~/bin/mysql.prc

studio_prc.o: $(pc)studio.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_studio_prc_start=resource_studio\
		$(pc)studio.prc studio_prc.o

conductor_prc.o: $(pc)conductor.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_conductor_prc_start=resource_conductor\
		$(pc)conductor.prc conductor_prc.o

midi_prc.o: $(pc)midi.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_midi_prc_start=resource_midi\
		$(pc)midi.prc midi_prc.o

http_prc.o: $(pc)http.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_http_prc_start=resource_http\
		$(pc)http.prc http_prc.o

store_prc.o: $(pc)store.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_store_prc_start=resource_store\
		$(pc)store.prc store_prc.o

f1_prc.o: $(pc)f1.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_f1_prc_start=resource_f1\
		$(pc)f1.prc f1_prc.o

help_prc.o: $(pc)help.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_help_prc_start=resource_help\
		$(pc)help.prc help_prc.o

record_prc.o: $(pc)record.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_record_prc_start=resource_record\
		$(pc)record.prc record_prc.o

neural_prc.o: $(pc)neural.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_neural_prc_start=resource_neural\
		$(pc)neural.prc neural_prc.o

keyboard_prc.o: $(pc)keyboard.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_keyboard_prc_start=resource_keyboard\
		$(pc)keyboard.prc keyboard_prc.o

sql_prc.o: $(pc)sql.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_sql_prc_start=resource_sql\
		$(pc)sql.prc sql_prc.o

test_prc.o: $(pc)test.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_test_prc_start=resource_test\
		$(pc)test.prc test_prc.o

xml_prc.o: $(pc)xml.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_xml_prc_start=resource_xml\
		$(pc)xml.prc xml_prc.o

distribution_prc.o: $(pc)distribution.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_distribution_prc_start=resource_distribution\
		$(pc)distribution.prc distribution_prc.o

control_prc.o: $(pc)control.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_control_prc_start=resource_control\
		$(pc)control.prc control_prc.o

prcgtk_prc.o: $(pc)prcgtk.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_prcgtk_prc_start=resource_prcgtk\
		$(pc)prcgtk.prc prcgtk_prc.o

~/bin/studio: encoder.o $(objects) prc.o prolog_neural.o prolog_conductor.o midi_command_prompt.o $(prc_resources)
	g++ -Wl,--export-dynamic encoder.o $(objects) $(prc_resources) prc.o \
	-o ~/bin/studio -lreadline -lpthread -ldl

studio: ~/bin/studio
	cp ~/bin/studio studio

/usr/lib/prcgtk.so: $(pc)prcgtk.cpp prcgtk_prc.o
	g++ -Wno-write-strings -fPIC -shared prcgtk_prc.o $(pc)prcgtk.cpp -o /usr/lib/prcgtk.so `pkg-config --cflags gtk+-2.0 --libs gtk+-2.0`

prcgtk: /usr/lib/prcgtk.so

