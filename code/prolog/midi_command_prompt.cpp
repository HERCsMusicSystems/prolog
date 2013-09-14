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

#include "midi_command_prompt.h"

MidiCommandPrompt :: MidiCommandPrompt (void) {
	#ifdef WINDOWS_OPERATING_SYSTEM
	output = NULL;
	input = NULL;
	thread = NULL;
	#endif
	#ifdef LINUX_OPERATING_SYSTEM
	thread = 0;
	#endif
}

MidiCommandPrompt :: ~ MidiCommandPrompt (void) {}

#ifdef WINDOWS_OPERATING_SYSTEM

static DWORD WINAPI midi_command_prompt_runner (LPVOID parameter) {
	while (true) {((MidiCommandPrompt *) parameter) -> run ();}
}

void MidiCommandPrompt :: run (void) {
	midi_stream * line = getLine ();
	if (line == 0) return;
	DWORD x;
	ReadFile (input, area, AREA_SIZE_1, & x, NULL);
	area [x] = '\0';
	line -> open_system_exclusive ();
	line -> insert (0x28);
	line -> insert (area);
	line -> close_system_exclusive ();
}

void MidiCommandPrompt :: print (char * text) {
	if (output == NULL) return;
	DWORD x;
	WriteFile (output, text, (DWORD) strlen (text), & x, NULL);
}

void MidiCommandPrompt :: open (void) {
	if (output != NULL) return;
	AllocConsole ();
	SetConsoleTitle ("HERCs PROLOG CONSOLE");
	output = GetStdHandle (STD_OUTPUT_HANDLE);
	input = GetStdHandle (STD_INPUT_HANDLE);
	SetConsoleTextAttribute (output, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	DWORD id;
	thread = CreateThread (NULL, 0, midi_command_prompt_runner, this, 0, & id);
}

void MidiCommandPrompt :: close (void) {
	if (output == NULL) return;
	TerminateThread (thread, 0);
	CloseHandle (thread);
	CloseHandle (output);
	CloseHandle (input);
	FreeConsole ();
	output = NULL;
	input = NULL;
	thread = NULL;
}

void MidiCommandPrompt :: setColors (int foreground, int background) {
	if (output == NULL) return;
	WORD attribute = 0;
	if (foreground & 1) attribute |= FOREGROUND_BLUE;
	if (foreground & 2) attribute |= FOREGROUND_GREEN;
	if (foreground & 4) attribute |= FOREGROUND_RED;
	if (foreground & 8) attribute |= FOREGROUND_INTENSITY;
	if (background & 1) attribute |= BACKGROUND_BLUE;
	if (background & 2) attribute |= BACKGROUND_GREEN;
	if (background & 4) attribute |= BACKGROUND_RED;
	if (background & 8) attribute |= BACKGROUND_INTENSITY;
	SetConsoleTextAttribute (output, attribute);
}

#endif

#ifdef LINUX_OPERATING_SYSTEM

static void * command_runner (void * parameter) {
	while (true) {
		((MidiCommandPrompt *) parameter) -> run ();
	}
	return 0;
}

void MidiCommandPrompt :: run (void) {
	int ind = 0;
	int ch = getchar ();
	while (ch >= ' ') {
		ind = area_cat (area, ind, ch);
		ch = getchar ();
	}
	area_cat (area, ind, "\0");
	midi_stream * line = getLine ();
	if (line == 0) return;
	line -> open_system_exclusive ();
	line -> insert (0x28);
	line -> insert (area);
	line -> close_system_exclusive ();
}

/*
#include <readline/readline.h>
#include <readline/history.h>

void MidiCommandPrompt :: run (void) {
	char * area = readline ("");
	add_history (area);
	line -> open_system_exclusive ();
	line -> insert (0x28);
	line -> insert (area);
	line -> insert (0);
	line -> close_system_exclusive ();
	usleep (delay);
}
*/

void MidiCommandPrompt :: print (char * text) {
	printf ("%s", text);
}

void MidiCommandPrompt :: open (void) {
	pthread_create (& thread, 0, command_runner, this);
	pthread_detach (thread);
}

void MidiCommandPrompt :: close (void) {pthread_cancel (thread);}

void MidiCommandPrompt :: setColors (int foreground, int background) {
	bool foreground_intensity = foreground > 7;
	bool background_intensity = background > 7;
	int foreground_red = (foreground & 0x4) ? (foreground_intensity ? 5 : 2) : 0;
	int foreground_green = (foreground & 0x2) ? (foreground_intensity ? 5 : 2) : 0;
	int foreground_blue = (foreground & 0x1) ? (foreground_intensity ? 5 : 2) : 0;
	int background_red = (background & 0x4) ? (background_intensity ? 5 : 2) : 0;
	int background_green = (background & 0x2) ? (background_intensity ? 5 : 2) : 0;
	int background_blue = (background & 0x1) ? (background_intensity ? 5 : 2) : 0;
	int fg = 16 + foreground_blue + 6 * foreground_green + 36 * foreground_red;
	int bg = 16 + background_blue + 6 * background_green + 36 * background_red;
	printf ("%c[38;5;%d;48;5;%dm", 27, fg, bg);
}

#endif

