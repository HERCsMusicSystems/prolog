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

#include "windows_midi.h"

#include <windows.h>
#include <mmsystem.h>

class windows_dx_midi_transmitter_class : public midi_stream {
public:
	HMIDIOUT out_port_handle;
	bool out_active;
	unsigned char buffer [1024];
	int buffer_pointer;
	void discharge (void) {
//		HANDLE hbuffer = GlobalAlloc (GHND, buffer_pointer);
//		if (hbuffer) {
			MIDIHDR midihdr;
//			midihdr . lpData = (LPSTR) GlobalLock (hbuffer);
			midihdr . lpData = (LPSTR) buffer;
//			if (midihdr . lpData) {
				midihdr . dwBufferLength = buffer_pointer;
				midihdr . dwFlags = 0;
				UINT err = midiOutPrepareHeader (out_port_handle, & midihdr, sizeof (MIDIHDR));
				if (! err) {
//					memcpy (midihdr . lpData, buffer, buffer_pointer);
					err = midiOutLongMsg (out_port_handle, & midihdr, sizeof (MIDIHDR));
					if (err) {
						char errmsg [120];
						midiOutGetErrorText (err, errmsg, 120);
						printf ("Error: %s%\n", errmsg);
					}
					while (MIDIERR_STILLPLAYING == midiOutUnprepareHeader (out_port_handle, & midihdr, sizeof (MIDIHDR))) {
						Sleep (10);
					}
				}
//				GlobalUnlock (hbuffer);
//			}
//			GlobalFree (hbuffer);
//		}
		buffer_pointer = 0;
	}
public:
	virtual void insert (int v1, int v2) {
		if (v1 >= 0xf0) {buffer [0] = (unsigned char) v1; buffer [1] = (unsigned char) v2; buffer_pointer = 2; discharge (); return;}
		if (out_active) midiOutShortMsg (out_port_handle, v1 | (v2 << 8));
	}
	virtual void insert (int v1, int v2, int v3) {if (out_active) midiOutShortMsg (out_port_handle, v1 | (v2 << 8) | (v3 << 16));}
	virtual void internal_insert (int value) {
		switch (value) {
		case 0xf0: case 0xf1: case 0xf2: case 0xf3: buffer_pointer = 0; buffer [buffer_pointer++] = (unsigned char) value; break;
		default:
			buffer [buffer_pointer++] = (unsigned char) value;
			if (value > 0xf3) discharge ();
			else {
				if (buffer_pointer == 1 && (buffer [0] == 0xf1 || buffer [0] == 0xf3)) discharge ();
				if (buffer_pointer == 2 && buffer [0] == 0xf2) discharge ();
			}
			break;
		}
	}
	virtual bool message_waiting (void) {return false;}
	windows_dx_midi_transmitter_class (void) {buffer_pointer = 0;}
};

class windows_dx_midi_receiver_class {
public:
	bool own_line;
	HMIDIIN in_port_handle;
	buffered_midi_stream * line;
	midi_reader * reader;
	MIDIHDR hdr1;
	MIDIHDR hdr2;
	unsigned char sxb1 [1024];
	unsigned char sxb2 [1024];
	windows_dx_midi_receiver_class (void) {own_line = true; line = new buffered_midi_stream (2096);}
	windows_dx_midi_receiver_class (buffered_midi_stream * line) {own_line = false; this -> line = line;}
	~ windows_dx_midi_receiver_class (void) {if (own_line && line != NULL) delete line; line = NULL;}
};

void windows_midi_service :: set_external_midi_in_line (buffered_midi_stream * line) {this -> receiver -> line = line;}

windows_midi_service :: windows_midi_service () {
	int ind;
	number_of_inputs = midiInGetNumDevs ();
	MIDIINCAPS in_info;
	for (ind = 0; ind < number_of_inputs; ind++) {
		midiInGetDevCaps (ind, & in_info, sizeof (MIDIINCAPS));
		prolog_string_copy (input_names [ind], in_info . szPname);
	}
	number_of_outputs = midiOutGetNumDevs ();
	MIDIOUTCAPS out_info;
	for (ind = 0; ind < number_of_outputs; ind++) {
		midiOutGetDevCaps (ind, & out_info, sizeof (MIDIOUTCAPS));
		prolog_string_copy (output_names [ind], out_info . szPname);
	}
	input_index = -1;
	output_index = -1;
	receiver = new windows_dx_midi_receiver_class ();
	receiver -> reader = NULL;
	transmitter = new windows_dx_midi_transmitter_class ();
}

windows_midi_service :: windows_midi_service (buffered_midi_stream * line) {
	int ind;
	number_of_inputs = midiInGetNumDevs ();
	MIDIINCAPS in_info;
	for (ind = 0; ind < number_of_inputs; ind++) {
		midiInGetDevCaps (ind, & in_info, sizeof (MIDIINCAPS));
		prolog_string_copy (input_names [ind], in_info . szPname);
	}
	number_of_outputs = midiOutGetNumDevs ();
	MIDIOUTCAPS out_info;
	for (ind = 0; ind < number_of_outputs; ind++) {
		midiOutGetDevCaps (ind, & out_info, sizeof (MIDIOUTCAPS));
		prolog_string_copy (output_names [ind], out_info . szPname);
	}
	input_index = -1;
	output_index = -1;
	receiver = new windows_dx_midi_receiver_class (line);
	receiver -> reader = NULL;
	transmitter = new windows_dx_midi_transmitter_class ();
}

windows_midi_service :: ~ windows_midi_service (void) {
	if (transmitter != NULL) delete transmitter; transmitter = NULL;
	if (receiver != NULL) delete receiver; receiver = NULL;
}

midi_stream * windows_midi_service :: getTransmissionLine (void) {return transmitter;}
midi_stream * windows_midi_service :: getReceptionLine (void) {return receiver -> line;}

int windows_midi_service :: getNumberOfInputs (void) {return number_of_inputs;}

int windows_midi_service :: getNumberOfOutputs (void) {return number_of_outputs;}

char * windows_midi_service :: getInputInfo (int ind) {
	if (ind < 0 || ind >= number_of_inputs) return NULL;
	return input_names [ind];
}

char * windows_midi_service :: getOutputInfo (int ind) {
	if (ind < 0 || ind >= number_of_outputs) return NULL;
	return output_names [ind];
}

int windows_midi_service :: getInputPort (void) {return input_index;}

int windows_midi_service :: getOutputPort (void) {return output_index;}

static void CALLBACK windows_dx_midi_in_procedure (HMIDIIN hMidiIn, UINT msg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
	int d1, d2, d3;
	windows_dx_midi_receiver_class * rec = (windows_dx_midi_receiver_class *) ((DWORDLONG) dwInstance);
	switch (msg) {
	case MIM_DATA:
		d1 = dwParam1 & 0xff;
		d2 = (dwParam1 >> 8) & 0xff;
		d3 = (dwParam1 >> 16) & 0xff;
		switch (d1 >> 4) {
		case 0x8: case 0x9: case 0xa: case 0xb: case 0xe:
			rec -> line -> insert (d1, d2, d3);
			break;
		case 0xc: case 0xd:
			rec -> line -> insert (d1, d2);
			break;
		case 0xf:
			switch (d1) {
			case 0xf1: case 0xf3: rec -> line -> insert (d1, d2); break;
			case 0xf2: rec -> line -> insert (d1, d2, d3); break;
			default: rec -> line -> insert (d1); break;
			}
			break;
		default: break;
		}
		break;
	case MIM_LONGDATA:
		{
			MIDIHDR * mhdr = (MIDIHDR *) dwParam1;
			int count = (int) mhdr -> dwBytesRecorded;
			unsigned char * data = (unsigned char *) mhdr -> lpData;
			rec -> line -> open_generic_system_exclusive ();
			for (int ind = 1; ind < count - 1; ind++) {
				rec -> line -> insert (data [ind]);
			}
			rec -> line -> close_system_exclusive ();
			midiInPrepareHeader (rec -> in_port_handle, mhdr, sizeof (MIDIHDR));
			midiInAddBuffer (rec -> in_port_handle, mhdr, sizeof (MIDIHDR));
		}
		break;
	default: break;
	}
	if (rec -> reader == NULL) return;
	if (rec -> reader -> is_ready ()) rec -> reader -> read (rec -> line);
}

bool windows_midi_service :: setInputPort (int ind) {
	if (ind >= number_of_inputs) return false;
	if (ind < -1) ind = -1;
	if (ind == input_index) return true;
	if (input_index >= 0) {
		// ....
		midiInStop (receiver -> in_port_handle);
		if (midiInClose (receiver -> in_port_handle) != MMSYSERR_NOERROR) {
			printf ("Failed to close input [%i]\n", input_index);
			return false;
		}
		midiInUnprepareHeader (receiver -> in_port_handle, & receiver -> hdr1, sizeof (receiver -> hdr1));
		midiInUnprepareHeader (receiver -> in_port_handle, & receiver -> hdr2, sizeof (receiver -> hdr2));
	}
	input_index = -1;
	if (ind >= 0) {
		// ....
		if (midiInOpen (& receiver -> in_port_handle, ind, (DWORD_PTR) windows_dx_midi_in_procedure, (DWORD_PTR) receiver, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
			printf ("Failed to open [%i]\n", ind);
			return false;
		}
		receiver -> hdr1 . lpData = (LPSTR) receiver -> sxb1;
		receiver -> hdr1 . dwBufferLength = sizeof (receiver -> sxb1);
		receiver -> hdr1 . dwBytesRecorded = 0L;
		receiver -> hdr1 . dwFlags = 0L;
		receiver -> hdr1 . dwUser = 0L;
		receiver -> hdr2 . lpData = (LPSTR) receiver -> sxb2;
		receiver -> hdr2 . dwBufferLength = sizeof (receiver -> sxb2);
		receiver -> hdr2 . dwBytesRecorded = 0L;
		receiver -> hdr2 . dwFlags = 0L;
		receiver -> hdr2 . dwUser = 0L;
		midiInPrepareHeader (receiver -> in_port_handle, & receiver -> hdr1, sizeof (receiver -> hdr1));
		midiInPrepareHeader (receiver -> in_port_handle, & receiver -> hdr2, sizeof (receiver -> hdr2));
		midiInAddBuffer (receiver -> in_port_handle, & receiver -> hdr1, sizeof (receiver -> hdr1));
		midiInAddBuffer (receiver -> in_port_handle, & receiver -> hdr2, sizeof (receiver -> hdr2));
		midiInStart (receiver -> in_port_handle);
	}
	input_index = ind;
	return true;
}

bool windows_midi_service :: setOutputPort (int ind) {
	if (ind >= number_of_outputs) return false;
	if (ind < -1) ind = -1;
	if (ind == output_index) return true;
	if (output_index >= 0) {
		if (midiOutClose (transmitter -> out_port_handle) != MMSYSERR_NOERROR) {
			printf ("Failed to close output [%i]\n", output_index);
			return false;
		}
		transmitter -> out_active = false;
	}
	output_index = -1;
	if (ind >= 0) {
		if (midiOutOpen (& transmitter -> out_port_handle, ind, NULL, NULL, CALLBACK_NULL) != MMSYSERR_NOERROR) {
			printf ("Failed to open output [%i]\n", ind);
			return false;
		}
		transmitter -> out_active = true;
	}
	output_index = ind;
	return true;
}

void windows_midi_service :: set_reader (midi_reader * reader) {
	if (receiver == NULL) return;
	receiver -> reader = reader;
}

void windows_midi_service :: changeManufacturersId (void) {if (receiver == NULL) return; receiver -> line -> set_manufacturers_id ();}
void windows_midi_service :: changeManufacturersId (int ind) {if (receiver == NULL) return; receiver -> line -> set_manufacturers_id (ind);}
void windows_midi_service :: changeManufacturersId (int ind, int sub) {if (receiver == NULL) return; receiver -> line -> set_manufacturers_id (ind, sub);}
void windows_midi_service :: changeProductId (char id1, char id2, char id3, char id4) {if (receiver == NULL) return; receiver -> line -> set_product_id (id1, id2, id3, id4);}
void windows_midi_service :: changeProductVersion (char id1, char id2, char id3, char id4) {if (receiver == NULL) return; receiver -> line -> set_product_version (id1, id2, id3, id4);}
