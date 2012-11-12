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

#include "prolog_linux_console.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static void * command_runner (void * parameter) {
	while (true) {
		((PrologLinuxConsole *) parameter) -> run ();
	}
	return 0;
}

void PrologLinuxConsole :: print (char * text) {
	printf ("%s", text);
}

void PrologLinuxConsole :: run (void) {
	int ind = 0;
	int ch = getchar ();
	while (ch >= ' ') {
		ind = area_cat (area, ind, ch);
		ch = getchar ();
	}
	area_cat (area, ind, "\n");
	insert (area);
	usleep (delay);
}

#include <readline/readline.h>
#include <readline/history.h>

//void PrologLinuxConsole :: run (void) {
//	char * area = readline (prompt);
//	if (strlen (area) > 2) add_history (area);
//	insert (area);
//	usleep (delay);
//}

PrologLinuxConsole :: PrologLinuxConsole (void) {
	threadable = false;
	configure ();
}

PrologLinuxConsole :: PrologLinuxConsole (int horizontal) {
	threadable = true;
	if (horizontal < 1) horizontal = 1;
	delay = 1000000 / horizontal;
	configure ();
}

PrologLinuxConsole :: PrologLinuxConsole (int horizontal, int seconds) {
	threadable = true;
	if (seconds < 1) seconds = 1;
	if (horizontal < 1) horizontal = 1;
	seconds *= 1000000;
	delay = seconds / horizontal;
	configure ();
}

void PrologLinuxConsole :: configure (void) {
	thread = 0;
	strcpy (prompt, "");
}

bool PrologLinuxConsole :: empty (void) {
	bool is_empty = PrologCommand :: empty ();
	if (threadable || ! is_empty) return is_empty;
	char * area = readline (prompt);
	if (strlen (area) > 2) {
		HIST_ENTRY * * histories = history_list ();
		if (histories != NULL && history_length > 0) {
			if (strcmp (area, histories [history_length - 1] -> line) != 0) add_history (area);
		} else add_history (area);
	}
	insert (area);
	insert ("\n");
	return PrologCommand :: empty ();
}

char * PrologLinuxConsole :: getPrompt (void) {return prompt;}
void PrologLinuxConsole :: setPrompt (char * prompt) {prolog_string_copy (this -> prompt, prompt);}

void PrologLinuxConsole :: open (void) {
	if (threadable) {
		if (thread != 0) return;
		pthread_attr_t attr;
		pthread_attr_init (& attr);
		pthread_attr_setstacksize (& attr, 120 * 1024);
		pthread_attr_setdetachstate (& attr, PTHREAD_CREATE_DETACHED);
		pthread_create (& thread, & attr, command_runner, this);
		pthread_attr_destroy (& attr);
		return;
	}
	char command [256];
	sprintf (command, "%s/.prc_history", getenv ("HOME"));
	stifle_history (128);
	read_history (command);
}

void PrologLinuxConsole :: close (void) {
	if (thread == 0) return;
	pthread_cancel (thread);
}

void PrologLinuxConsole :: stop (void) {
	if (! threadable) {
		char command [256];
		sprintf (command, "%s/.prc_history", getenv ("HOME"));
		write_history (command);
	}
	if (thread == 0) return;
	pthread_cancel (thread);
}

PrologLinuxConsole :: ~ PrologLinuxConsole (void) {}

void PrologLinuxConsole :: setColors (int foreground, int background) {
	int foreground_red = (foreground >> 16) & 0xff;
	int foreground_green = (foreground >> 8) & 0xff;
	int foreground_blue = foreground & 0xff;
	int background_red = (background >> 16) & 0xff;
	int background_green = (background >> 8) & 0xff;
	int background_blue = background & 0xff;
	int fg = 16 + foreground_blue / 43 + 6 * (foreground_green / 43) + 36 * (foreground_red / 43);
	int bg = 16 + background_blue / 43 + 6 * (background_green / 43) + 36 * (background_red / 43);
	printf ("%c[38;5;%d;48;5;%dm", 27, fg, bg);
}

