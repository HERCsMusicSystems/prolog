///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2016 Robert P. Wolf                       //
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

package prolog;

public class Encoder {
	public int rnd_value = 0, rnd_a = 17, rnd_c = 3, rnd_mod = 16384;
	public int get_rnd () {rnd_value *= rnd_a; rnd_value += rnd_c; rnd_value %= rnd_mod; return rnd_value;}
	public int char_to_int (char ch) {
		if (ch >= '0' && ch <= '9') return ch - '0';
		if (ch == '-') return 10;
		if (ch >= 'A' && ch <= 'Z') return 11 + ch - 'A';
		if (ch == '/') return 37;
		if (ch >= 'a' && ch <= 'z') return 38 + ch - 'a';
		return ch;
	}
	public char int_to_char (int ind) {
		if (ind >= 0 && ind <= 9) return (char) (ind + '0');
		if (ind == 10) return '-';
		if (ind >= 11 && ind <= 36) return (char) (ind - 11 + 'A');
		if (ind == 37) return '/';
		if (ind >= 38 && ind <= 63) return (char) (ind - 38 + 'a');
		return (char) ind;
	}
	public int check_char_to_int (char ch) {
		if (ch == '-') return 0;
		if (ch == '/') return 1;
		if (ch >= '0' && ch <= '9') return 2 + ch - '0';
		if (ch >= 'A' && ch <= 'Z') return 12 + ch - 'A';
		if (ch >= 'a' && ch <= 'z') return 38 + ch - 'a';
		return ch;
	}
	public char int_to_check_char (int ind) {
		if (ind == 0) return (char) '-';
		if (ind == 1) return (char) '/';
		if (ind < 12) return (char) ('0' + ind - 2);
		if (ind < 38) return (char) ('A' + ind - 12);
		return (char) ('a' + ind - 38);
	}
	public char check_character (String code) {
		int sub = 0;
		byte [] cs = code . getBytes ();
		for (byte ch : cs) sub += check_char_to_int ((char) ch);
		sub &= 63;
		if (sub != 0) sub = 64 - sub;
		return int_to_check_char (sub);
	}
	public int check_position (String code) {
		int sub = 0;
		byte [] cs = code . getBytes ();
		int sentinel = cs . length;
		for (byte ch : cs) sub += char_to_int ((char) ch);
		return sentinel != 0 ? sub % sentinel : sub;
	}
	public int extract_volume (String serial) {
		if (serial . length () < 9) return 0;
		int ret = 0;
		int ch;
		for (int ind = 3; ind < 9; ind++) {
			ret <<= 4;
			ch = serial . charAt (ind);
			if (ch >= 'A') ch -= 'A' - 10;
			else ch -= '0';
			ret += ch;
		}
		return ret;
	}
	public boolean check_serial (String serial, String key, int shift) {
		String commander = calculate_key (serial, shift);
		if (commander . length () < 1 || key . length () < 1) return false;
		return commander . equals (key);
	}
	public boolean check_serial (String serial, int volume, String key, int shift) {return check_serial (volumise_serial (serial, volume), key, shift);}
	public boolean validate_serial (String serial) {
		if (serial . length () != 12) return false;
		int check = 0;
		for (int ind = 0; ind < 10; ind++) check += serial . charAt (ind);
		check &= 0xff;
		int msb = serial . charAt (10);
		if (msb >= 'A') msb -= 'A' - 10; else msb -= '0';
		int lsb = serial . charAt (11);
		if (lsb >= 'A') lsb -= 'A' - 10; else lsb -= '0';
		int ind = (msb << 4) + lsb;
		return check == ind;
	}
	public boolean validate_key (String key) {return check_character (key) == '-';}
	public String create_serial_number (String header, int serial) {
		serial &= 0xffffff;
		int checksum = 0;
		for (byte ch : String . format ("%s-%06X-", header, serial) . getBytes ()) checksum += ch;
		checksum &= 0xff;
		return String . format ("%s-%06X-%02X", header, serial, checksum);
	}
	public String normalise_serial (String serial) {
		if (serial . length () != 12) return serial;
		serial = serial . substring (0, 10);
		int checksum = 0;
		for (byte ch : serial . getBytes ()) checksum += ch;
		checksum &= 0xff;
		return String . format ("%s%02X", serial, checksum);
	}
	public String volumise_serial (String serial, int volume) {
		if (serial . length () != 12) return serial;
		return create_serial_number (serial . substring (0, 2), volume ^ extract_volume (serial));
	}
	public String calculate_key (String in, int shift) {
		int position = check_position (in);
		int sentinel = in . length ();
		for (int ind = 0; ind < sentinel; ind++) shift += char_to_int (in . charAt (ind)) * (ind + 1);
		rnd_value = shift % rnd_mod;
		sentinel--;
		String accu = "";
		for (int ind = 0; ind < sentinel; ind++) accu += int_to_char ((get_rnd () ^ char_to_int (in . charAt (ind))) & 63);
		String out = "";
		for (int ind = 0; ind < sentinel; ind++) {
			if (ind == position) out += check_character (accu);
			out += accu . charAt (ind);
		}
		if (sentinel == position) out += check_character (accu);
		return out;
	}
}

