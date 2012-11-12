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
#include "midi_stream.h"
#include <windows.h>

#ifndef _PROLOG_HERCS_CONSOLE_
#define _PROLOG_HERCS_CONSOLE_

class HERCs_PROLOG_SDK_EXPORT PrologHercsConsole;

class PrologHercsConsole : public PrologCommand {
private:
	midi_stream * line;
	HANDLE output;
	HANDLE input;
	volatile bool running;
	AREA area;
	int delay;
	void configure (midi_stream * line);
public:
	virtual void print (char * text);
	virtual void open (void);
	virtual void close (void);
	virtual void setColors (int foreground, int background);
	void run (void);
	PrologHercsConsole (midi_stream * line, int horizontal);
	PrologHercsConsole (midi_stream * line, int horizontal, int seconds);
	~ PrologHercsConsole (void);
};

#endif
