///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2003 Robert P. Wolf                       //
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

#ifndef _MIDI_STREAM
#define _MIDI_STREAM

#include <pthread.h>

class midi_stream {
private:
	pthread_mutex_t locker;
private:
	int manufacturers_id_1;
	int manufacturers_id_2;
	char product_id [4];
	char product_version [4];
private:
	int checksum;
	int get_checksum;
	int last_message;
	int last_get;
	int marked_last_get;
	int marked_get_checksum;
	int channel_extension;
	int write_channel_extension;
public:
	midi_stream * thru;
	midi_stream * next;
protected:
	virtual int internal_get (void);
	virtual int internal_get_command (void);
	virtual void internal_insert (int value);
	virtual void internal_close_message (void);
	virtual void internal_insert_command (int value);
	virtual void internal_insert_running_command (int value);
	virtual void internal_mark (void);
	virtual void internal_restore (void);
	virtual int internal_get_data_xor (void);
	virtual void internal_ready (void);
public:
	void close_message (void);
	void mark (void);
	void restore (void);
	void set_channel_extension (int extension);
	int get_channel_extension (void);
	int chex (int channel);
	virtual bool message_waiting (void);
	virtual void insert_manufacturers_id (void);
	virtual void insert_product_id (void);
	virtual void insert_product_version (void);
public:
	void set_manufacturers_id (void);
	void set_manufacturers_id (int ind);
	void set_manufacturers_id (int ind, int sub);
	void set_product_id (char * id);
	void set_product_id (char id1, char id2, char id3, char id4);
	void set_product_version (char * id);
	void set_product_version (char id1, char id2, char id3, char id4);
public:
	virtual void lock (void);
	virtual void unlock (void);
	int get_command (void);
	int get (void);
	int get (char * text);
	int get_f7 (void);
	int get_int (void);
	int get_negative (void);
	int get_very_negative (void);
	int get_short_negative (void);
	int get_int128 (void);
	bool get_boolean (void);
	bool check_system_exclusive (void);
	bool check_checksum (void);
	void insert (int value);
	virtual void insert (int v1, int v2);
	virtual void insert (int v1, int v2, int v3);
	void insert (char * text);
	void insert_command (int value);
	void insert_running_command (int value);
	void insert_keyoff (int channel, int key, int velocity = 0);
	void insert_keyon (int channel, int key, int velocity);
	void insert_pat (int channel, int key, int value);
	void insert_control (int channel, int control, int value);
	void insert_control (int channel, int control, int msb, int lsb);
	void insert_nrpn (int channel);
	void insert_nrpn (int channel, int msb_data);
	void insert_nrpn (int channel, int msb, int lsb);
	void insert_nrpn (int channel, int msb, int lsb, int msb_data);
	void insert_nrpn (int channel, int msb, int lsb, int msb_data, int lsb_data);
	void insert_nrpn_14 (int channel, int data);
	void insert_nrpn_14 (int channel, int msb, int lsb, int data);
	void insert_rpn (int channel);
	void insert_rpn (int channel, int delta);
	void insert_rpn (int channel, int msb, int lsb);
	void insert_rpn (int channel, int msb, int lsb, int delta);
	void insert_programchange (int channel, int program);
	void insert_cat (int channel, int value);
	void insert_pitchbend (int channel, int value);
	void insert_pitchbend (int channel, int msb, int lsb);
	void insert_channel_command (int command);
	void open_system_exclusive (void);
	void open_generic_system_exclusive (void);
	void insert_checksum (void);
	void insert_data_xor (void);
	void insert_channel_extension (int extension);
	void close_system_exclusive (void);
	void connect_thru (midi_stream * thru = 0);
	void disconnect_thru (void);
	void ready (void);
	midi_stream (void);
	virtual ~ midi_stream (void);
};

class buffered_midi_stream : public midi_stream {
protected:
	char * data;
	int size;
	int from;
	int to;
	int marked_position;
	int command;
	int complete_message;
protected:
	virtual int internal_get (void);
	virtual int internal_get_command (void);
	virtual void internal_insert (int value);
	virtual void internal_close_message (void);
	virtual void internal_mark (void);
	virtual void internal_restore (void);
	virtual int internal_get_data_xor (void);
public:
	virtual bool message_waiting (void);
	buffered_midi_stream (int size);
	virtual ~ buffered_midi_stream (void);
};

class clearable_midi_stream : public buffered_midi_stream {
protected:
	bool running_command_not_required;
	virtual void internal_insert_running_command (int value);
public:
	void clear (void);
	clearable_midi_stream (int size);
};

class delayed_buffered_midi_stream : public buffered_midi_stream {
protected:
	int * delays;
public:
	virtual bool message_waiting (void);
	virtual void internal_insert (int value);
	void insert_delay (int delay);
	delayed_buffered_midi_stream (int size);
	~ delayed_buffered_midi_stream (void);
};

class midi_reader {
public:
	int midi_channel_extension;
	int active_sensed;
	int active_sensing_delay;
public:
	void read (midi_stream * line, int samples = 1);
	virtual void midi_keyoff (int channel, int key);
	virtual void midi_keyoffv (int channel, int key, int velocity);
	virtual void midi_keyon (int channel, int key, int velocity);
	virtual void midi_pat (int channel, int key, int value);
	virtual void midi_control (int channel, int controller, int value);
	virtual void midi_programchange (int channel, int program);
	virtual void midi_cat (int channel, int value);
	virtual void midi_pitchbend (int channel, int v1, int v2);
	virtual void midi_system_exclusive (midi_stream * line);
	virtual void midi_parameter_request (int channel);
	virtual void midi_timing_clock (void);
	virtual void midi_start (void);
	virtual void midi_continue (void); // pause
	virtual void midi_stop (void);
	virtual void midi_active_sensing (void);
	virtual void midi_active_sensed (void);
	virtual bool is_ready (void);
	midi_reader (void);
};

class midi_nrpn_reader : public midi_reader {
private:
	int midi_channel;
	int reception_channel_extension;
	int nrpn_msb;
	int nrpn_lsb;
	int data_lsb;
public:
	virtual void midi_control (int channel, int controller, int value);
	virtual void midi_controller (int controller, int value);
	virtual void midi_nrpn (int nrpn_msb, int nrpn_lsb, int data_msb, int data_lsb);
	void set_midi_channel (int channel);
	void set_midi_channel_all (void);
	void set_midi_channel_off (void);
	void set_midi_channel_extension (int extension);
	void cancel_midi_channel_extension (void);
	int get_reception_channel (void); // 0..15, 16=all, 0x7f=off
	int get_transmission_channel (void); // 0..15, 0=all, 15=off
	int get_transmission_channel (int extension);
	int get_channel_extension (void);
	midi_nrpn_reader (void);
};

#endif
