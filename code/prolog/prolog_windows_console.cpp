///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2007 Robert P. Wolf                       //
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

#include "prolog_windows_console.h"

#include "process.h"

void PrologWindowsConsole :: print (char * text) {
	DWORD x;
	WriteFile (output, text, (DWORD) strlen (text), & x, NULL);
}

int PrologWindowsConsole :: get (void) {
	int ch = PrologCommand :: get ();
	if (ch >= 0) return ch;
	if (input == 0) return ch;
	print (prompt);
	while (ch < 0) {
		DWORD end;
		ReadFile (input, area, AREA_SIZE_1, & end, 0);
		area [end] = '\0';
		insert (area);
		ch = PrologCommand :: get ();
	}
	return ch;
}

PrologWindowsConsole :: PrologWindowsConsole (void) {
	strcpy (prompt, "");
	output = NULL;
	input = NULL;
	AllocConsole ();
	SetConsoleTitle ("HERCs PROLOG CONSOLE");
	output = GetStdHandle (STD_OUTPUT_HANDLE);
	input = GetStdHandle (STD_INPUT_HANDLE);
	current_foreground = 0xffff00;
	current_background = 0;
	SetConsoleTextAttribute (output, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

PrologWindowsConsole :: ~ PrologWindowsConsole (void) {
//	if (output != 0) CloseHandle (output); output = 0;
//	if (input != 0) CloseHandle (input); input = 0;
//	FreeConsole ();
}

char * PrologWindowsConsole :: getPrompt (void) {return prompt;}
void PrologWindowsConsole :: setPrompt (char * prompt) {prolog_string_copy (this -> prompt, prompt);}

void PrologWindowsConsole :: setForeground (int foreground) {setColours (current_foreground = foreground, current_background);}
void PrologWindowsConsole :: setBackground (int background) {setColours (current_foreground, current_background = background);}

void PrologWindowsConsole :: setColours (int foreground, int background) {
	if (output == NULL) return;
	int foreground_red = (foreground >> 16) & 0xff;
	int foreground_green = (foreground >> 8) & 0xff;
	int foreground_blue = foreground & 0xff;
	bool foreground_intensity = foreground_red > 128 || foreground_green > 128 || foreground_blue > 128;
	int background_red = (background >> 16) & 0xff;
	int background_green = (background >> 8) & 0xff;
	int background_blue = background & 0xff;
	bool background_intensity = background_red > 128 || background_green > 128 || background_blue > 128;
	WORD attribute = 0;
	if (foreground_red >= 128) attribute |= FOREGROUND_RED;
	if (foreground_blue >= 128) attribute |= FOREGROUND_BLUE;
	if (foreground_green >= 128) attribute |= FOREGROUND_GREEN;
	if (foreground_intensity) attribute |= FOREGROUND_INTENSITY;
	if (background_red >= 128) attribute |= BACKGROUND_RED;
	if (background_blue >= 128) attribute |= BACKGROUND_BLUE;
	if (background_green >= 128) attribute |= BACKGROUND_GREEN;
	if (background_intensity) attribute |= BACKGROUND_INTENSITY;
	SetConsoleTextAttribute (output, attribute);
}
