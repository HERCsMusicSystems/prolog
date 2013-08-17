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

#ifndef _ENCODER_
#define _ENCODER_

class encoder {
private:
	int rnd_value;
	int rnd_a;
	int rnd_c;
	int rnd_mod;
	int get_rnd (void);
	int char_to_int (char ch);
	char int_to_char (int ind);
	int check_char_to_int (char ch);
	char int_to_check_char (int ind);
	char check_character (char * code);
	int check_position (char * code);
	unsigned long int extract_volume (char * serial);
public:
	bool check_serial (char * serial, char * key, int shift = 0);
	bool check_serial (char * serial, unsigned long int volume, char * key, int shift = 0);
	bool validate_serial (char * serial);
	bool validate_key (char * key);
	void create_serial_number (char * out, char * header, unsigned long int volume);
	void normalize_serial (char * out, char * serial);
	void volumize_serial (char * out, char * serial, unsigned long int volume);
	void calculate_key (char * out, char * in, int shift = 0);
	encoder (void);
};

#endif
