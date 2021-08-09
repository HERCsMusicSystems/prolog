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

#ifndef _PROLOG_STUDIO_SERVICE_CLASS_
#define _PROLOG_STUDIO_SERVICE_CLASS_

#include "prolog.h"

class standard_in_reader : public PrologReader {
protected:
	void message (char * text);
	void message_v (char * text, char * variable);
	int move_z (void);
};

class term_reader : public PrologReader {
public:
	char * text;
	virtual void message (char * text);
	virtual int move_z (void);
	void init (PrologRoot * root, char * text);
};

class symbol_reader : public PrologReader {
public:
	FILE * fi;
	void message (char * text);
	void message_v (char * text, char * variable);
	int move_z (void);
	void init (PrologRoot * root, FILE * fi);
};

class PrologNoise TRACK {
public:
	int a, c, m, v, range;
	int get (void);
	int get (int min, int max);
	double get (double min, double max);
	void resolution (int bits);
	void control (int v);
	void control (int a, int c);
	PrologNoise (void);
};

class PrologStudio : public PrologServiceClass {
private:
	PrologRoot * root;
	PrologDirectory * directory;
	standard_in_reader stdr;
	PrologNoise n;
public:
	virtual void init (PrologRoot * root, PrologDirectory * directory);
	virtual PrologNativeCode * getNativeCode (char * name);
};

#endif

