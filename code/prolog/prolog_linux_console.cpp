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

#include <readline/readline.h>
#include <readline/history.h>

PrologLinuxConsole :: PrologLinuxConsole (void) {
	strcpy (prompt, "");
	char command [256];
	sprintf (command, "%s/.prc_history", getenv ("HOME"));
	stifle_history (128);
	read_history (command);
}

PrologLinuxConsole :: ~ PrologLinuxConsole (void) {
	insert (area);
	insert ("\n");
	char command [256];
	sprintf (command, "%s/.prc_history", getenv ("HOME"));
	write_history (command);
}

void PrologLinuxConsole :: print (char * text) {
	printf ("%s", text);
}

#ifdef MAC_OPERATING_SYSTEM
AREA previous_line = "";
#endif

int PrologLinuxConsole :: get (void) {
	int ch = PrologCommand :: get ();
	if (ch >= 0) return ch;
	while (ch < 0) {
		char * area = readline (prompt);
		if (strlen (area) > 2) {
			HIST_ENTRY * * histories = history_list ();
			if (histories != 0 && history_length > 0) {
				if (strcmp (area, histories [history_length - 1] -> line) != 0) add_history (area);
			} else add_history (area);
		}
		insert (area);
		insert ("\n");
		ch = PrologCommand :: get ();
	}
	return ch;
}

char * PrologLinuxConsole :: getPrompt (void) {return prompt;}
void PrologLinuxConsole :: setPrompt (char * prompt) {prolog_string_copy (this -> prompt, prompt);}

void PrologLinuxConsole :: setColours (int foreground, int background) {
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

