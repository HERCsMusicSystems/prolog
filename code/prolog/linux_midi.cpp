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

#include "linux_midi.h"

#include <pthread.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/times.h>
#include <fcntl.h>

typedef void * (* runner_procedure) (void * parameter);
static void beginthread (runner_procedure runner, int value, linux_midi_service * root) {
	pthread_t threader;
	pthread_attr_t attr;
	pthread_attr_init (& attr);
	pthread_attr_setstacksize (& attr, 120 * 1024);
	pthread_attr_setdetachstate (& attr, PTHREAD_CREATE_DETACHED);
	pthread_create (& threader, & attr, runner, root);
	pthread_attr_destroy (& attr);
}

static void * linux_midi_runner (void * parameter) {
	linux_midi_service * servo = (linux_midi_service *) parameter;
	unsigned char command = 0;
	unsigned char channel = 0;
	unsigned char v1, v2;
	int res;
	while (true) {
		if (servo -> midi_input_id < 0) usleep (10000);
		else {
			if (res = read (servo -> midi_input_id, & v1, 1) < 1) {usleep (10000); continue;}
			if (servo -> reader_line == NULL) continue;
			if (v1 < 128) {
				if ((command >= 0x80 && command < 0xc0) || (command >= 0xe0 && command < 0xf0)) {
					res = read (servo -> midi_input_id, & v2, 1);
					servo -> reader_line -> insert (command, v1, v2);
				} else {servo -> reader_line -> insert (command, v1);}
			} else {
				command = v1; channel = command & 0xf;
				if ((command >= 0x80 && command < 0xc0) || (command >= 0xe0 && command < 0xf0)) {
					res = read (servo -> midi_input_id, & v1, 1);
					res = read (servo -> midi_input_id, & v2, 1);
					servo -> reader_line -> insert (command, v1, v2);
				} else {
					if (command < 0xf0) {
						res = read (servo -> midi_input_id, & v1, 1);
						servo -> reader_line -> insert (command, v1);
					} else {
						switch (command) {
						case 0xf0:
							servo -> reader_line -> open_generic_system_exclusive ();
							res = read (servo -> midi_input_id, & v1, 1);
							while (v1 != 0xf7) {
								servo -> reader_line -> insert (v1);
								res = read (servo -> midi_input_id, & v1, 1);
							}
							servo -> reader_line -> close_system_exclusive ();
							break;
						case 0xf1:
							res = read (servo -> midi_input_id, & v1, 1);
							servo -> reader_line -> insert (command, v1);
							break;
						case 0xf3:
							res = read (servo -> midi_input_id, & v1, 1);
							servo -> reader_line -> insert (command, v1);
							break;
						case 0xf2:
							res = read (servo -> midi_input_id, & v1, 1);
							res = read (servo -> midi_input_id, & v2, 1);
							servo -> reader_line -> insert (command, v1, v2);
							break;
						default: servo -> reader_line -> insert (command); break;
						}
					}
				}
			}
			if (servo -> reader != NULL && servo -> reader -> is_ready ()) servo -> reader -> read (servo -> reader_line);
		}
	}
	return 0;
}

class linux_midi_transmitter_class : public midi_stream {
public:
	int stream_id;
	virtual void internal_insert (int value) {
		if (stream_id < 0) return;
		unsigned char data [1]; data [0] = value;
		int res = write (stream_id, data, sizeof (data));
	}
	virtual bool message_waiting (void) {return false;}
	linux_midi_transmitter_class (void) {stream_id = -1;}
};

static char * device_name_0 = "/dev/midi0";
static char * device_name_1 = "/dev/midi1";
static char * device_name_2 = "/dev/midi2";
static char * device_name_3 = "/dev/midi3";
static char * device_name_4 = "/dev/midi4";
static char * device_name_5 = "/dev/midi5";
static char * device_name_6 = "/dev/midi6";
static char * device_name_7 = "/dev/midi7";
static char * device_name_8 = "/dev/midi8";
static char * device_name_9 = "/dev/midi9";

char * linux_midi_service :: getInputInfo (int ind) {return getOutputInfo (ind);}
char * linux_midi_service :: getOutputInfo (int ind) {
	switch (ind) {
	case 0: return device_name_0;
	case 1: return device_name_1;
	case 2: return device_name_2;
	case 3: return device_name_3;
	case 4: return device_name_4;
	case 5: return device_name_5;
	case 6: return device_name_6;
	case 7: return device_name_7;
	case 8: return device_name_8;
	case 9: return device_name_9;
	default: return "unknown";
	}
	return "unknown";
}
int linux_midi_service :: getNumberOfInputs (void) {return getNumberOfOutputs ();}
int linux_midi_service :: getNumberOfOutputs (void) {return 10;}

int linux_midi_service :: getInputPort (void) {return selected_input_device;}
int linux_midi_service :: getOutputPort (void) {return selected_output_device;}
bool linux_midi_service :: setInputPort (int ind) {
	if (ind > 9) return false;
	if (midi_input_id >= 0) close (midi_input_id);
	midi_input_id = selected_input_device = -1;
	if (ind < 0) return true;
	midi_input_id = open (getOutputInfo (ind), O_RDONLY);
	if (midi_input_id < 0) {midi_input_id = -1; selected_input_device = -1; return false;}
	selected_input_device = ind;
	return true;
}
bool linux_midi_service :: setInputPort (char * location) {
	if (midi_input_id >= 0) close (midi_input_id);
	midi_input_id = selected_input_device = -1;
	midi_input_id = open (location, O_RDONLY);
	if (midi_input_id < 0) {midi_input_id = selected_input_device = -1; return false;}
	selected_input_device = 10;
	return true;
}
bool linux_midi_service :: setOutputPort (int ind) {
	if (transmitter == NULL) return false;
	if (ind > 9) return false;
	if (transmitter -> stream_id >= 0) close (transmitter -> stream_id);
	transmitter -> stream_id = -1;
	selected_output_device = -1;
	if (ind < 0) return true;
	transmitter -> stream_id = open (getOutputInfo (ind), O_WRONLY);
	if (transmitter -> stream_id < 0) {transmitter -> stream_id = -1; selected_output_device = -1; return false;}
	selected_output_device = ind;
	return true;
}
bool linux_midi_service :: setOutputPort (char * location) {
	if (transmitter == NULL) return false;
	if (transmitter -> stream_id >= 0) close (transmitter -> stream_id);
	transmitter -> stream_id = selected_output_device = -1;
	transmitter -> stream_id = open (location, O_WRONLY);
	if (transmitter -> stream_id < 0) {transmitter -> stream_id = selected_output_device = -1; return false;}
	selected_output_device = 10;
	return true;
}

midi_stream * linux_midi_service :: getTransmissionLine (void) {return transmitter;}
midi_stream * linux_midi_service :: getReceptionLine (void) {return reader_line;}
void linux_midi_service :: set_reader (midi_reader * reader) {this -> reader = reader;}

linux_midi_service :: linux_midi_service (void) {
	midi_input_id = selected_input_device = selected_output_device = -1;
	transmitter = new linux_midi_transmitter_class ();
	reader = NULL;
	reader_line = NULL;
	beginthread (linux_midi_runner, 0, this);
}

linux_midi_service :: ~ linux_midi_service (void) {
	if (midi_input_id >= 0) close (midi_input_id);
	if (transmitter != NULL) {
		if (transmitter -> stream_id >= 0) {
			close (transmitter -> stream_id);
		}
		delete transmitter;
	}
}

void linux_midi_service :: changeManufacturersId (void) {if (reader_line == NULL) return; reader_line -> set_manufacturers_id ();}
void linux_midi_service :: changeManufacturersId (int ind) {if (reader_line == NULL) return; reader_line -> set_manufacturers_id (ind);}
void linux_midi_service :: changeManufacturersId (int ind, int sub) {if (reader_line == NULL) return; reader_line -> set_manufacturers_id (ind, sub);}
void linux_midi_service :: changeProductId (char id1, char id2, char id3, char id4) {if (reader_line == NULL) return; reader_line -> set_product_id (id1, id2, id3, id4);}
void linux_midi_service :: changeProductVersion (char id1, char id2, char id3, char id4) {if (reader_line == NULL) return; reader_line -> set_product_version (id1, id2, id3, id4);}

