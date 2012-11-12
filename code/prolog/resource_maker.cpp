///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2008 Robert P. Wolf                       //
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

#include <stdio.h>

int main (int args, char * * argv) {
//	for (int ind = 0; ind < args; ind++) {
//		printf ("	argv [%i %s]\n", ind, argv [ind]);
//	}
	if (args < 2) {
		printf ("Usage: resource_maker <resource_file> <source_file(s)> ....\n");
		return 0;
	}
	FILE * tc = fopen (argv [1], "wb");
	if (! tc) {
		printf ("Can not open output file [%s]\n", argv [1]);
		return 0;
	}
	int file_index = 2;
	FILE * fr;
	int ch;
	int resource_size;
	while (file_index < args) {
		fr = fopen (argv [file_index], "rb");
		if (fr) {
			fprintf (tc, "char resource_%i [] = {", file_index - 2);
			resource_size = 0;
			ch = fgetc (fr);
			while (ch != EOF) {
				if (resource_size > 0) fprintf (tc, ", ");
				fprintf (tc, "0x%02x", ch);
				ch = fgetc (fr);
				resource_size++;
			}
			fprintf (tc, "};\n");
			fprintf (tc, "#define resource_size_%i %i\n", file_index - 2, resource_size);
			fclose (fr);
		}
		file_index++;
	}
	fclose (tc);
}
