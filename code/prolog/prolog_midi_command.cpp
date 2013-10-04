///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2005 Robert P. Wolf                       //
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

#include <string.h>
#include "prolog_midi_command.h"

PrologMidiCommand :: PrologMidiCommand (midi_stream * line) : PrologCommand () {
	this -> line = line;
	previous_char = 0;
	sem_init (& semaphore, 0, 1);
}

PrologMidiCommand :: ~ PrologMidiCommand (void) {sem_destroy (& semaphore);}

int PrologMidiCommand :: get (void) {
	if (previous_char < 0) sem_wait (& semaphore);
	previous_char = PrologCommand :: get ();
	return previous_char < 0 ? 13 : previous_char;
}

void PrologMidiCommand :: insert (char * text) {
	PrologCommand :: insert (text);
	sem_post (& semaphore);
}

void PrologMidiCommand :: insert_midi (int cc, int mm, int ll) {
	line -> open_system_exclusive ();
	line -> insert (0x29);
	line -> insert (cc);
	line -> insert (mm);
	line -> insert (ll);
	line -> close_system_exclusive ();
}

void PrologMidiCommand :: print (char * text) {
	AREA area;
	char * cp = text;
	strncpy (area, cp, 100); area [100] = '\0';
	int length = (int) strlen (area);
	cp = & cp [length];
	while (length > 0) {
		line -> open_system_exclusive ();
		line -> insert (* area < 0 ? 0x2b : 0x2a);
		line -> insert (area);
		line -> close_system_exclusive ();
		strncpy (area, cp, 100); area [100] = '\0';
		length = (int) strlen (area);
		cp = & cp [length];
	}
}

void PrologMidiCommand :: open (void) {insert_midi (0, 0, 0);}
void PrologMidiCommand :: close (void) {insert_midi (4, 0, 0);}
void PrologMidiCommand :: setColours (int foreground, int background) {
//	insert_midi (0x0b, foreground, background);
	int foreground_red = (foreground >> 16) & 0xff;
	int foreground_green = (foreground >> 8) & 0xff;
	int foreground_blue = foreground & 0xff;
	bool foreground_intensity = foreground_red > 128 || foreground_green > 128 || foreground_blue > 128;
	foreground = 0;
	if (foreground_intensity) foreground += 8;
	if (foreground_red >= 128) foreground += 4;
	if (foreground_green >= 128) foreground += 2;
	if (foreground_blue >= 128) foreground += 1;
	int background_red = (background >> 16) & 0xff;
	int background_green = (background >> 8) & 0xff;
	int background_blue = background & 0xff;
	bool background_intensity = background_red > 128 || background_green > 128 || background_blue > 128;
	background = 0;
	if (background_intensity) background += 8;
	if (background_red >= 128) background += 4;
	if (background_green >= 128) background += 2;
	if (background_blue >= 128) background += 1;
	insert_midi (0x0b, foreground, background);
}
void PrologMidiCommand :: openEditor1 (void) {insert_midi (1, 0, 0);}
void PrologMidiCommand :: openEditor2 (int selector) {insert_midi (2, selector, 0);}
void PrologMidiCommand :: openEditor3 (int selector, int sub_selector) {insert_midi (3, selector, sub_selector);}
void PrologMidiCommand :: closeEditor1 (void) {insert_midi (5, 0, 0);}
void PrologMidiCommand :: closeEditor2 (int selector) {insert_midi (6, selector, 0);}
void PrologMidiCommand :: closeEditor3 (int selector, int sub_selector) {insert_midi (7, selector, sub_selector);}
void PrologMidiCommand :: setScreenCoordinates (int x, int y) {
	insert_midi (12, x >> 7, x & 0x7f);
	insert_midi (13, y >> 7, y & 0x7f);
}
