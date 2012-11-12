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

#include "mac_midi.h"

extern void midi_in_proc (const MIDIPacketList * pktlist, void * readProcRefCon, void * srcConnRefCon);

void freeSysex (MIDISysexSendRequest * pk) {
	Byte * pt = (Byte *) pk;
	delete pt;
}

class dx_midi_transmit_out_messages_class2 : public midi_stream {
public:
	MIDIClientRef client;
	MIDIPortRef out_port;
	MIDIEndpointRef out_endpoint;
	MIDIPortRef in_port;
	MIDIEndpointRef in_endpoint;
	MIDIEndpointRef virtual_midi_out;
	MIDIEndpointRef virtual_midi_in;
public:
	Byte buffer [8192];
	int buffer_pointer;
//	void discharge (void) {
//		discharge (out_endpoint);
//		discharge (virtual_midi_out);
//		buffer_pointer = 0;
//	}
	void discharge (void) {
		if (buffer_pointer == 0) return;
		if (out_endpoint != NULL) {
			MIDISysexSendRequest * pk = (MIDISysexSendRequest *) (new Byte [sizeof (MIDISysexSendRequest) + buffer_pointer + 16]);
			Byte * data = (Byte *) pk + sizeof (MIDISysexSendRequest);
			pk -> complete = false;
			pk -> destination = out_endpoint;
			pk -> data = data;
			pk -> bytesToSend = buffer_pointer;
			pk -> completionProc = freeSysex;
			pk -> completionRefCon = 0;
			for (int ind = 0; ind < buffer_pointer; ind++) * (data++) = buffer [ind];
			MIDISendSysex (pk);
			buffer_pointer = 0;
			return;
		}
		if (buffer_pointer > 250) {buffer_pointer = 0; return;}
		MIDIPacketList packet;
		packet . numPackets = 1;
		packet . packet [0] . timeStamp = 0;
		for (int ind = 0; ind < buffer_pointer; ind++) {
			packet . packet [0] . data [ind] = buffer [ind];
		}
		packet . packet [0] . length = buffer_pointer;
		MIDIReceived (virtual_midi_out, & packet);
		buffer_pointer = 0;
	}
	void insert_one (int v) {
		buffer_pointer = 0;
		buffer [buffer_pointer++] = v;
		discharge ();
	}
	virtual void insert (int v1, int v2) {
//		if (out_endpoint == NULL) return;
		MIDIPacketList packet;
		packet . numPackets = 1;
		packet . packet [0] . timeStamp = 0;
		packet . packet [0] . length = 2;
		packet . packet [0] . data [0] = v1;
		packet . packet [0] . data [1] = v2;
		if (out_endpoint != NULL) MIDISend (out_port, out_endpoint, & packet);
		MIDIReceived (virtual_midi_out, & packet);
	}
	virtual void insert (int v1, int v2, int v3) {
//		if (out_endpoint == NULL) return;
		MIDIPacketList packet;
		packet . numPackets = 1;
		packet . packet [0] . timeStamp = 0;
		packet . packet [0] . length = 3;
		packet . packet [0] . data [0] = v1;
		packet . packet [0] . data [1] = v2;
		packet . packet [0] . data [2] = v3;
		if (out_endpoint != NULL) MIDISend (out_port, out_endpoint, & packet);
		MIDIReceived (virtual_midi_out, & packet);
	}
	virtual void internal_insert (int value) {
		switch (value) {
		case 0xf0:
				buffer_pointer = 0;
				buffer [buffer_pointer++] = 0xf0;
				break;
		case 0xf7: buffer [buffer_pointer++] = 0xf7; discharge (); break;
		case 0xf8: case 0xfe: insert_one (value); break;
		default:
				if (buffer [0] == 0xf1 && buffer_pointer == 1) {insert (0xf1, value); break;}
				if (buffer [0] == 0xf3 && buffer_pointer == 1) {insert (0xf3, value); break;}
				if (buffer [0] == 0xf2 && buffer_pointer == 2) {insert (0xf2, buffer [1], value); break;}
				if (value > 0xf3) {insert_one (value); break;}
				buffer [buffer_pointer++] = value;
				if (buffer_pointer > 819) buffer_pointer = 0;
			break;
		}
	}
	dx_midi_transmit_out_messages_class2 (void) {
		buffer_pointer = 0;
		in_endpoint = NULL;
		out_endpoint = NULL;
		in_port = NULL;
		out_port = NULL;
	}
};

class dx_midi_transmit_out_messages_class : public midi_reader {
private:
	Byte buffer [65536];
	int buffer_pointer;
public:
	MIDIClientRef client;
	MIDIPortRef in_port;
	MIDIEndpointRef in_endpoint;
	MIDIPortRef out_port;
	MIDIEndpointRef out_endpoint;
public:
	dx_midi_transmit_out_messages_class (void) {buffer_pointer = 0;}
public:
	void discharge (void) {
		if (buffer_pointer == 0) return;
		if (out_endpoint == NULL) {buffer_pointer = 0; return;}
		buffer [buffer_pointer++] = 0xf0;
		buffer [buffer_pointer++] = 0xf7;
		MIDISysexSendRequest * pk = (MIDISysexSendRequest *) (new Byte [sizeof (MIDISysexSendRequest) + buffer_pointer + 16]);
		Byte * data = (Byte *) pk + sizeof (MIDISysexSendRequest);
		pk -> complete = false;
		pk -> destination = out_endpoint;
		pk -> data = data;
		pk -> bytesToSend = buffer_pointer;
		pk -> completionProc = freeSysex;
		pk -> completionRefCon = 0;
		for (int ind = 0; ind < buffer_pointer; ind++) * (data++) = buffer [ind];
		buffer_pointer = 0;
		MIDISendSysex (pk);
	}
	void send_two (Byte one, Byte two) {buffer [buffer_pointer++] = one; buffer [buffer_pointer++] = two;}
	void send_three (Byte one, Byte two, Byte three) {buffer [buffer_pointer++] = one; buffer [buffer_pointer++] = two; buffer [buffer_pointer++] = three;}
	/*void emit (MIDIPacketList * packet) {if (out_endpoint == NULL) return; MIDISend (out_port, out_endpoint, packet);}
	void send_two (Byte one, Byte two) {
		MIDIPacketList packet;
		packet . numPackets = 1;
		packet . packet [0] . timeStamp = 0;
		packet . packet [0] . length = 2;
		packet . packet [0] . data [0] = one;
		packet . packet [0] . data [1] = two;
		emit (& packet);
	}
	void send_three_standard (Byte one, Byte two, Byte three) {
		MIDIPacketList packet;
		packet . numPackets = 1;
		packet . packet [0] . timeStamp = 0;
		packet . packet [0] . length = 3;
		packet . packet [0] . data [0] = one;
		packet . packet [0] . data [1] = two;
		packet . packet [0] . data [2] = three;
		emit (& packet);
	}
	void send_three (Byte one, Byte two, Byte three) {
		if (out_endpoint == NULL) return;
		MIDISysexSendRequest * pk = (MIDISysexSendRequest *) (new Byte [sizeof (MIDISysexSendRequest) + 8]);
		Byte * data = (Byte *) pk + sizeof (MIDISysexSendRequest);
		pk -> complete = false;
		pk -> destination = out_endpoint;
		pk -> data = data;
		pk -> bytesToSend = 3;
		pk -> completionProc = freeSysex;
		pk -> completionRefCon = 0;
		* (data++) = one;
		* (data++) = two;
		* data = three;
		MIDISendSysex (pk);
	}*/
public:
	virtual void midi_keyoff (int channel, int key) {send_three (0x80 + channel, key, 0);}
	virtual void midi_keyon (int channel, int key, int velocity) {send_three (0x90 + channel, key, velocity);}
	virtual void midi_pat (int channel, int key, int value) {send_three (0xa0 + channel, key, value);}
	virtual void midi_control (int channel, int controller, int value) {send_three (0xb0 + channel, controller, value);}
	virtual void midi_programchange (int channel, int program) {send_two (0xc0 + channel, program);}
	virtual void midi_cat (int channel, int value) {send_two (0xd0 + channel, value);}
	virtual void midi_pitchbend (int channel, int v1, int v2) {send_three (0xe0 + channel, v1, v2);}
	virtual void midi_parameter_request (int channel) {send_two (0xf1, channel); if (buffer_pointer > 180) discharge ();}
	virtual void midi_timing_clock (void) {buffer [buffer_pointer++] = 0xf8;}
	virtual void midi_system_exclusive (midi_stream * line) {
		buffer [buffer_pointer++] = 0xf0;
		int dat = line -> get ();
		while (dat != 0xf7) {
			buffer [buffer_pointer++] = (Byte) dat;
			dat = line -> get ();
		}
		buffer [buffer_pointer++] = 0xf7;
		if (buffer_pointer > 180) discharge ();
	}
};
//		MIDIPacketList packet;
//		packet . numPackets = 1;
//		packet . packet [0] . timeStamp = 0;
//		packet . packet [0] . length = 1;
//		packet . packet [0] . data [0] = 0xf8;
//		emit (& packet);
//	}
/*	virtual void midi_system_exclusive (midi_stream * line) {
		if (out_endpoint == NULL) {
			line -> get_f7 ();
			return;
		}
		line -> mark ();
		int size = 2;
		while (line -> get () != 0xf7) size++;
		line -> restore ();
		MIDISysexSendRequest * pk = (MIDISysexSendRequest *) (new Byte [sizeof (MIDISysexSendRequest) + size + 64]);
		Byte * data = (Byte *) pk + sizeof (MIDISysexSendRequest);
		pk -> complete = false;
		pk -> destination = out_endpoint;
		pk -> data = data;
		pk -> bytesToSend = size;
		pk -> completionProc = freeSysex;
		pk -> completionRefCon = 0;
		* (data++) = 0xf0;
		int dat = line -> get ();
		while (dat != 0xf7) {
			* (data++) = (Byte) dat;
			dat = line -> get ();
		}
		* data = 0xf7;
		MIDISendSysex (pk);
	}
};*/

int mac_midi_service :: getNumberOfInputs (void) {return MIDIGetNumberOfSources ();}
int mac_midi_service :: getNumberOfOutputs (void) {return MIDIGetNumberOfDestinations ();}

char * mac_midi_service :: getInputInfo (int ind) {
	if (ind < 0) return NULL;
	if (ind >= 32) return NULL;
	if (ind >= MIDIGetNumberOfSources ()) return NULL;
	MIDIEndpointRef end_point = MIDIGetSource (ind);
	CFStringRef name;
	MIDIObjectGetStringProperty (end_point, kMIDIPropertyName, & name);
	int sub = 0, index = 0;
	AREA area;
	while (CFStringGetCharacterAtIndex (name, sub) != '\0') index = area_cat (area, index, (char) CFStringGetCharacterAtIndex (name, sub++));
	prolog_string_copy (input_names [ind], area);
	return input_names [ind];
}

char * mac_midi_service :: getOutputInfo (int ind) {
	if (ind < 0) return NULL;
	if (ind >= 32) return NULL;
	if (ind >= MIDIGetNumberOfDestinations ()) return NULL;
	MIDIEndpointRef end_point = MIDIGetDestination (ind);
	CFStringRef name;
	MIDIObjectGetStringProperty (end_point, kMIDIPropertyName, & name);
	int sub = 0, index = 0;
	AREA area;
	while (CFStringGetCharacterAtIndex (name, sub) != '\0') index = area_cat (area, index, CFStringGetCharacterAtIndex (name, sub++));
	prolog_string_copy (output_names [ind], area);
	return output_names [ind];
}

int mac_midi_service :: getInputPort (void) {return input_index;}
int mac_midi_service :: getOutputPort (void) {return output_index;}
bool mac_midi_service :: setInputPort (int ind) {
	if (input_index >= 0) MIDIPortDisconnectSource (t2 -> in_port, t2 -> in_endpoint);
	if (ind < 0) {t2 -> in_endpoint = NULL; input_index = -1; return true;}
	if (ind >= MIDIGetNumberOfSources ()) return false;
	t2 -> in_endpoint = MIDIGetSource (ind);
	MIDIPortConnectSource (t2 -> in_port, t2 -> in_endpoint, NULL);
	input_index = ind;
	return true;
}
bool mac_midi_service :: setOutputPort (int ind) {
	if (ind < 0) {t2 -> out_endpoint = NULL; output_index = -1; return true;}
	if (ind >= MIDIGetNumberOfDestinations ()) return false;
	t2 -> out_endpoint = MIDIGetDestination (ind);
	output_index = ind;
	return true;
}

void midi_in_proc (const MIDIPacketList * pktlist, void * readProcRefCon, void * srcConnRefCon) {
	mac_midi_service * service = (mac_midi_service *) readProcRefCon;
	if (service == NULL) return;
	if (service -> line == NULL) return;
	const MIDIPacket * packet = pktlist -> packet;
	for (int ind = 0; ind < pktlist -> numPackets; ind++) {
		if (packet -> data [0] == 0xf0 || service -> reading_system_exclusive) {
			service -> reading_system_exclusive = true;
			for (int sub = 0; sub < packet -> length; sub++) {
				if (packet -> data [sub] == 0xf7) service -> reading_system_exclusive = false;
				service -> line -> insert (packet -> data [sub]);
			}
		} else {
			switch (packet -> length) {
			case 1: service -> line -> insert (packet -> data [0]); break;
			case 2: service -> line -> insert (packet -> data [0], packet -> data [1]); break;
			case 3: service -> line -> insert (packet -> data [0], packet -> data [1], packet -> data [2]); break;
			default: for (int sub = 0; sub < packet -> length; sub++) service -> line -> insert (packet -> data [sub]); break;
			}
		}
	}
	if (service -> reader == NULL || service -> reading_system_exclusive) return;
	if (service -> reader -> is_ready ()) service -> reader -> read (service -> line);
}

mac_midi_service :: mac_midi_service (void) {
//	transmitter = new dx_midi_transmit_out_messages_class;
	t2 = new dx_midi_transmit_out_messages_class2 ();
	line = new buffered_midi_stream (8192);
	MIDIClientCreate (CFSTR ("HERCs Prolog"), NULL, 0, & t2 -> client);
	MIDIInputPortCreate (t2 -> client, CFSTR ("HERCs Prolog in port"), midi_in_proc, this, & t2 -> in_port);
	MIDIOutputPortCreate (t2 -> client, CFSTR ("HERCs Prolog out port"), & t2 -> out_port);
	MIDISourceCreate (t2 -> client, CFSTR ("HERCs Prolog Virtual MIDI Source"), & t2 -> virtual_midi_out);
	MIDIDestinationCreate (t2 -> client, CFSTR ("HERCs Prolog Virtual MIDI Destination"), midi_in_proc, this, & t2 -> virtual_midi_in);
	input_index = -1;
	output_index = -1;
	reading_system_exclusive = false;
}

mac_midi_service :: ~ mac_midi_service (void) {
//	if (transmitter != NULL) delete transmitter; transmitter = NULL;
	if (t2 != NULL) delete t2; t2 = NULL;
	if (line != NULL) delete line; line = NULL;
}

midi_stream * mac_midi_service :: getTransmissionLine (void) {return t2;}
midi_stream * mac_midi_service :: getReceptionLine (void) {return line;}

//void mac_midi_service :: send (midi_stream * line) {transmitter -> read (line); transmitter -> discharge ();}
//void mac_midi_service :: set_input_line (midi_stream * line) {this -> line = line;}
void mac_midi_service :: set_reader (midi_reader * reader) {this -> reader = reader;}

void mac_midi_service :: changeManufacturersId (void) {if (line == NULL) return; line -> set_manufacturers_id ();}
void mac_midi_service :: changeManufacturersId (int ind) {if (line == NULL) return; line -> set_manufacturers_id (ind);}
void mac_midi_service :: changeManufacturersId (int ind, int sub) {if (line == NULL) return; line -> set_manufacturers_id (ind, sub);}
void mac_midi_service :: changeProductId (char id1, char id2, char id3, char id4) {if (line == NULL) return; line -> set_product_id (id1, id2, id3, id4);}
void mac_midi_service :: changeProductVersion (char id1, char id2, char id3, char id4) {if (line == NULL) return; line -> set_product_version (id1, id2, id3, id4);}
