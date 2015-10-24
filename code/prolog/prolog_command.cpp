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

#include "prolog.h"

PrologCommand :: PrologCommand (void) TRACKING (19) {
	from = 0;
	to = 0;
}

PrologCommand :: ~ PrologCommand (void) {}

void PrologCommand :: insert (char * text) {
	int t = to;
	while (* text != '\0') {//> 31) {
		command [t++] = * text++;
		if (t > AREA_SIZE_1) t = 0;
	}
	to = t;
}

int PrologCommand :: get (void) {
	if (from == to) return -1;
	int c = command [from++];
	if (from > AREA_SIZE_1) from = 0;
	return c;
}

void PrologCommand :: print (int ch) {putchar (ch);}
void PrologCommand :: print (char * text) {printf ("%s", text);}
void PrologCommand :: setForeground (int foreground) {}
void PrologCommand :: setBackground (int background) {}
void PrologCommand :: setColours (int foreground, int background) {}
void PrologCommand :: openEditor1 (void) {}
void PrologCommand :: openEditor2 (int selector) {}
void PrologCommand :: openEditor3 (int selector, int sub_selector) {}
void PrologCommand :: closeEditor1 (void) {}
void PrologCommand :: closeEditor2 (int selector) {}
void PrologCommand :: closeEditor3 (int selector, int sub_selector) {}
void PrologCommand :: setScreenCoordinates (int x, int y) {}
char * PrologCommand :: getPrompt (void) {return NULL;}
void PrologCommand :: setPrompt (char * prompt) {}
void PrologCommand :: save_history (int size, char * file_name) {}
void PrologCommand :: load_history (int size, char * file_name) {}

