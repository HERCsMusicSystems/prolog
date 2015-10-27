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

#ifndef _PROLOG_WINDOWS_CONSOLE_
#define _PROLOG_WINDOWS_CONSOLE_

#include "prolog.h"
#include <windows.h>

class PrologWindowsConsole : public PrologCommand {
private:
	HANDLE output;
	HANDLE input;
	AREA area;
	PROLOG_STRING prompt;
	int current_foreground, current_background;
public:
	virtual void print (char * text);
	virtual int get (void);
	virtual void setForeground (int foreground);
	virtual void setBackground (int background);
	virtual void setColours (int foreground, int background);
	virtual char * getPrompt (void);
	virtual void setPrompt (char * prompt);
	PrologWindowsConsole (void);
	~ PrologWindowsConsole (void);
};

#endif
