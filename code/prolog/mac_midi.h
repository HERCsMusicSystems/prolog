///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2008 Robert P. Wolf                       //
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

#ifndef _MAC_MIDI_SERVICE_
#define _MAC_MIDI_SERVICE_

#include "hercs_prolog_sdk.h"
#include <CoreMIDI/MIDIServices.h>

class dx_midi_transmit_out_messages_class;
class dx_midi_transmit_out_messages_class2;

class mac_midi_service : public PrologMidiPortServiceClass {
private:
	PROLOG_STRING input_names [32];
	PROLOG_STRING output_names [32];
	dx_midi_transmit_out_messages_class2 * t2;
	int input_index;
	int output_index;
	buffered_midi_stream * own_reader_line;
public:
	midi_stream * reader_line;
	midi_reader * reader;
	bool reading_system_exclusive;
public:
	virtual int getNumberOfInputs (void);
	virtual int getNumberOfOutputs (void);
	virtual char * getInputInfo (int ind);
	virtual char * getOutputInfo (int ind);
	virtual int getInputPort (void);
	virtual int getOutputPort (void);
	virtual bool setInputPort (int ind);
	virtual bool setOutputPort (int ind);
	virtual midi_stream * getTransmissionLine (void);
	virtual midi_stream * getReceptionLine (void);
	void set_reader (midi_reader * reader);
	mac_midi_service (char * virtual_port_name);
	~ mac_midi_service (void);
public:
	virtual void changeManufacturersId (void);
	virtual void changeManufacturersId (int ind);
	virtual void changeManufacturersId (int ind, int sub);
	virtual void changeProductId (char id1, char id2, char id3, char id4);
	virtual void changeProductVersion (char id1, char id2, char id3, char id4);
};

#endif
