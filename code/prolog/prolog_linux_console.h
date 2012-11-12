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

#ifndef _PROLOG_LINUX_CONSOLE_
#define _PROLOG_LINUX_CONSOLE_

#include "prolog.h"
#include <pthread.h>

class HERCs_PROLOG_SDK_EXPORT PrologLinuxConsole;

class PrologLinuxConsole : public PrologCommand {
private:
	pthread_t thread;
	AREA area;
	int delay;
	void configure (void);
	PROLOG_STRING prompt;
	bool threadable;
public:
	virtual void print (char * text);
	virtual void open (void);
	virtual void close (void);
	virtual void setColors (int foreground, int background);
	virtual char * getPrompt (void);
	virtual void setPrompt (char * prompt);
	virtual bool empty (void);
	void run (void);
	void stop (void);
	PrologLinuxConsole (void);                           // with line history, without separate thread
	PrologLinuxConsole (int horizontal);                 // without line history, with separate thread
	PrologLinuxConsole (int horizontal, int seconds);    // without line history, with separate thread
	~ PrologLinuxConsole (void);
};

#endif
