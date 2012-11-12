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

#include "encoder.h"
#include <string.h>
#include <stdio.h>

int encoder :: get_rnd (void) {
	rnd_value *= rnd_a;
	rnd_value += rnd_c;
	rnd_value %= rnd_mod;
	return rnd_value;
}

int encoder :: char_to_int (char ch) {
	if (ch >= '0' && ch <= '9') return ch - '0';
	if (ch == '-') return 10;
	if (ch >= 'A' && ch <= 'Z') return 11 + ch - 'A';
	if (ch == '/') return 37;
	if (ch >= 'a' && ch <= 'z') return 38 + ch - 'a';
	return ch;
}

char encoder :: int_to_char (int ind) {
	if (ind >= 0 && ind <= 9) return (char) (ind + '0');
	if (ind == 10) return '-';
	if (ind >= 11 && ind <= 36) return (char) (ind - 11 + 'A');
	if (ind == 37) return '/';
	if (ind >= 38 && ind <= 63) return (char) (ind - 38 + 'a');
	return (char) ind;
}

int encoder :: check_char_to_int (char ch) {
	if (ch == '-') return 0;
	if (ch == '/') return 1;
	if (ch >= '0' && ch <= '9') return 2 + ch - '0';
	if (ch >= 'A' && ch <= 'Z') return 12 + ch - 'A';
	if (ch >= 'a' && ch <= 'z') return 38 + ch - 'a';
	return ch;
}

char encoder :: int_to_check_char (int ind) {
	if (ind == 0) return (char) '-';
	if (ind == 1) return (char) '/';
	if (ind < 12) return (char) ('0' + ind - 2);
	if (ind < 38) return (char) ('A' + ind - 12);
	return (char) ('a' + ind - 38);
}

char encoder :: check_character (char * code) {
	int sub = 0;
	int sentinel = (int) strlen (code);
	for (int ind = 0; ind < sentinel; ind++) sub += check_char_to_int (code [ind]);
	sub &= 63;
	if (sub != 0) sub = 64 - sub;
	return int_to_check_char (sub);
}

int encoder :: check_position (char * code) {
	int sub = 0;
	int sentinel = (int) strlen (code);
	for (int ind = 0; ind < sentinel; ind++) sub += char_to_int (code [ind]);
	return sentinel != 0 ? sub % sentinel : sub;
}

unsigned long int encoder :: extract_volume (char * serial) {
	if (strlen (serial) < 1) return 0;
	unsigned long int ret = 0;
	int ch;
	for (int ind = 3; ind < 9; ind++) {
		ret <<= 4;
		ch = serial [ind];
		if (ch >= 'A') ch -= 'A' - 10;
		else ch -= '0';
		ret += ch;
	}
	return ret;
}

bool encoder :: check_serial (char * serial, char * key, int shift) {
	char commander [64];
	calculate_key (commander, serial, shift);
	if (strlen (commander) == 0 || strlen (key) == 0) return false;
	return strcmp (commander, key) == 0;
}

bool encoder :: check_serial (char * serial, unsigned long int volume, char * key, int shift) {
	char command [64];
	if (strlen (serial) != 12) return false;
	volumize_serial (command, serial, volume);
	if (strcmp (command, serial) != 0) return false;
	return check_serial (command, key, shift);
}

bool encoder :: validate_serial (char * serial) {
	if (strlen (serial) != 12) return false;
	int check = 0;
	int ind = 0;
	for (ind = 0; ind < 10; ind++) check += serial [ind];
	check &= 0xff;
	int msb = serial [10];
	if (msb >= 'A') msb -= 'A' - 10;
	else msb -= '0';
	int lsb = serial [11];
	if (lsb >= 'A') lsb -= 'A' - 10;
	else lsb -= '0';
	ind = (msb << 4) + lsb;
	return check == ind;
}

bool encoder :: validate_key (char * key) {return check_character (key) == '-';}

void encoder :: create_serial_number (char * out, char * header, unsigned long int serial) {
	serial &= 0xffffff;
	sprintf (out, "%s-%06X-", header, (unsigned int) serial);
	int checksum = 0;
	char * ch = out;
	while (* ch != '\0') checksum += * (ch++);
	checksum &= 0xff;
	sprintf (out, "%s-%06X-%02X", header, (unsigned int) serial, checksum);
}

void encoder :: normalize_serial (char * out, char * serial) {
	strcpy (out, serial);
	if (strlen (serial) != 12) return;
	int checksum = 0;
	char ch;
	for (int ind = 0; ind < 10; ind++) {
		ch = serial [ind];
		checksum += ch;
		* (out++) = ch;
	}
	checksum &= 0xff;
	int msb = checksum >> 4;
	if (msb > 9) * (out++) = (char) ('A' + msb - 10);
	else * (out++) = (char) ('0' + msb);
	int lsb = checksum & 0xf;
	if (lsb > 9) * (out++) = (char) ('A' + lsb - 10);
	else * (out++) = (char) ('0' + lsb);
	* out = '\0';
}

void encoder :: volumize_serial (char * out, char * serial, unsigned long int volume) {
	strcpy (out, serial);
	if (strlen (serial) != 12) return;
	char header [4];
	header [0] = serial [0];
	header [1] = serial [1];
	header [2] = '\0';
	unsigned long int new_volume = extract_volume (serial);
	new_volume ^= volume;
	create_serial_number (out, header, new_volume);
}

void encoder :: calculate_key (char * out, char * in, int shift) {
	char accu [64] = "";
	int ind = 0;
	int sub = shift;
	int position = check_position (in);
	int sentinel = (int) strlen (in);
	for (ind = 0; ind < sentinel; ind++) sub += char_to_int (in [ind]) * (ind + 1);
	rnd_value = sub % rnd_mod;
	sentinel--;
	for (ind = 0; ind < sentinel; ind++) accu [ind] = int_to_char ((get_rnd () ^ char_to_int (in [ind])) & 63);
	accu [ind] = '\0';
	for (ind = 0; ind < sentinel; ind++) {
		if (ind == position) * (out++) = check_character (accu);
		* (out++) = accu [ind];
	}
	if (sentinel == position) * (out++) = check_character (accu);
	* out = '\0';
}

encoder :: encoder (void) {
	rnd_value = 0;
	rnd_a = 17;
	rnd_c = 3;
	rnd_mod = 16384;
}
