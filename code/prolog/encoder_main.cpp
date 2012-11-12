///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2004 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

#include "encoder.h"

#include <stdio.h>
#include <string.h>

int toInt (char * command) {
	if (command == NULL) return 0;
	int value = 0;
	while (* command >= '0' && * command <= '9') {value *= 10; value += * command++ - '0';}
	return value;
}

int main (int args, char * * argv) {

	if (args == 2 || args == 3) {
		encoder e;
		char serial [256];
		char key [256];
		e . normalize_serial (serial, argv [1]);
		if (args == 2) e . calculate_key (key, serial);
		else {int shift = toInt (argv [2]); e . calculate_key (key, serial, shift); printf ("shift [%i]\n", shift);}
		printf ("serial [%s]\n", serial);
		printf ("key [%s]\n", key);
		return 0;
	}

	if (args == 4) {
		char input_file [1024];
		char output_file [1024];
		char password [1024];
		strcpy (input_file, argv [1]);
		strcpy (password, argv [2]);
		strcpy (output_file, argv [3]);
		if (strlen (password) < 2) return 0;
		FILE * fr = fopen (input_file, "rb");
		if (fr == NULL) return 0;
		FILE * tc = fopen (output_file, "wb");
		if (fr == NULL) {fclose (fr); return 0;}
		char * pp = password;
		int ch = fgetc (fr);
		while (ch >= 0) {
			ch ^= (int) (* pp++);
			fputc (ch, tc);
			if (* pp == '\0') pp = password;
			ch = fgetc (fr);
		}
		fclose (fr); fclose (tc);
		return 0;
	}

	printf ("Usage: encoder XX-XXXXXX-?? [shift]\n");
	return 0;
}
