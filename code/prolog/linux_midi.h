///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2009 Robert P. Wolf                       //
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

#ifndef _LINUX_MIDI_
#define _LINUX_MIDI_

#include "hercs_prolog_sdk.h"

class linux_receiver_class;
class linux_midi_transmitter_class;

class linux_midi_service : public PrologMidiPortServiceClass {
private:
	linux_midi_transmitter_class * transmitter;
	int selected_input_device;
	int selected_output_device;
public:
	buffered_midi_stream * reader_line;
	midi_reader * reader;
	int midi_input_id;
public:
	virtual int getNumberOfInputs (void);
	virtual int getNumberOfOutputs (void);
	virtual char * getInputInfo (int ind);
	virtual char * getOutputInfo (int ind);
	virtual int getInputPort (void);
	virtual int getOutputPort (void);
	virtual bool setInputPort (int ind);
	virtual bool setInputPort (char * location);
	virtual bool setOutputPort (int ind);
	virtual bool setOutputPort (char * location);
	virtual midi_stream * getTransmissionLine (void);
	virtual midi_stream * getReceptionLine (void);
	void set_reader (midi_reader * reader);
	linux_midi_service (void);
	~ linux_midi_service (void);
public:
	virtual void changeManufacturersId (void);
	virtual void changeManufacturersId (int ind);
	virtual void changeManufacturersId (int ind, int sub);
	virtual void changeProductId (char id1, char id2, char id3, char id4);
	virtual void changeProductVersion (char id1, char id2, char id3, char id4);
};

#endif

