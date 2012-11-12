PHP_ARG_ENABLE(hrcs, whether to enable HRCS support,
[ --enable-hrcs   Enable HRCS support])

if test "$PHP_HRCS" = "yes"; then
  AC_DEFINE(HAVE_HRCS, 1, [Whether you have HRCS])
  PHP_REQUIRE_CXX()
  PHP_SUBST(HRCS_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, HRCS_SHARED_LIBADD)
  PHP_NEW_EXTENSION(hrcs, hrcs.cpp object_tracker.cpp prolog_midi_command.cpp prolog_reader.cpp midi_stream.cpp prolog_directory.cpp prolog_service_class.cpp prolog_dll_loader.cpp linux_midi.cpp prolog_query.cpp prolog_loader.cpp prolog_match.cpp prolog_element.cpp prolog_transport.cpp prolog_command.cpp midi_command_prompt.cpp prolog_native_code.cpp neural.cpp prolog_transport_pool.cpp encoder.cpp prolog_root.cpp prolog_resolution.cpp prolog_variable.cpp prolog_resources.cpp prolog_resolution_pool.cpp prolog_linux_console.cpp prolog_studio.cpp prolog_atom.cpp prolog_mysql.cpp, $ext_shared)
fi

