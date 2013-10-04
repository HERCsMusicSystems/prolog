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

#include "prolog.h"
#include "midi_stream.h"
#include "semaphore.h"

#ifndef _PROLOG_MIDI_COMMAND_
#define _PROLOG_MIDI_COMMAND_

class PrologMidiCommand;

class PrologMidiCommand : public PrologCommand {
private:
	midi_stream * line;
	void insert_midi (int cc, int mm, int ll);
	sem_t semaphore;
	int previous_char;
public:
	virtual void insert (char * text);
	virtual int get (void);
	virtual void print (char * text);
	virtual void open (void);
	virtual void close (void);
	virtual void setColours (int foreground, int background);
	virtual void openEditor1 (void);
	virtual void openEditor2 (int selector);
	virtual void openEditor3 (int selector, int sub_selector);
	virtual void closeEditor1 (void);
	virtual void closeEditor2 (int selector);
	virtual void closeEditor3 (int selector, int sub_selector);
	virtual void setScreenCoordinates (int x, int y);
	PrologMidiCommand (midi_stream * line);
	~ PrologMidiCommand (void);
};
#endif
