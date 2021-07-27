/usr/lib/BinaryJoker.so: BinaryJokerExe
	sudo cp BinaryJokerExe /usr/lib/BinaryJoker.so

BinaryJokerExe: BinaryJoker.o BinaryJokerPrc.o
	c++ -fPIC -shared BinaryJoker.o BinaryJokerPrc.o -o BinaryJokerExe

BinaryJoker.o: BinaryJoker.cpp
	c++ -Wno-write-strings -I../code/prolog -fPIC BinaryJoker.cpp -c -o BinaryJoker.o

BinaryJokerPrc.o: BinaryJokerPrc.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		BinaryJokerPrc.prc BinaryJokerPrc.o
	objdump -t BinaryJokerPrc.o


jack_prc.o: $(pc)jack.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_prolog_jack_prc_start=resource_jack\
		$(pc)jack.prc jack_prc.o



