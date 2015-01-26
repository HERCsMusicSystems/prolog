///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2014 Robert P. Wolf                       //
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

#include "joystick_linux.h"
#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <stdio.h>

void joystick :: move (double delay) {
	fd_set readset;
	FD_ZERO (& readset);
	FD_SET (fd, & readset);
	timeval timeout;
	timeout . tv_sec = 1;
	timeout . tv_usec = 0;
	if (select (fd + 1, & readset, 0, 0, & timeout) <= 0) return;
	struct js_event e;
	while (read (fd, & e, sizeof (e)) > 0) {
		if ((e . type & 0x3) == 1) button (e . number, e . value != 0);
		else {
			if (e . value == 32767) axis (e . number, 1.0);
			else if (e . value == -32767) axis (e . number, -1.0);
			else axis (e . number, (double) e . value / 32768.0);
		}
	}
}

void joystick :: button (int ind, bool value) {
	switch (ind) {
	case 0: fire_callback (fire = value); break;
	case 1: autofire_callback (autofire = value); break;
	case 2: h1_callback (h1 = value); break;
	case 3: h2_callback (h2 = value); break;
	case 4: h3_callback (h3 = value); break;
	case 5: h4_callback (h4 = value); break;
	case 6: e1_callback (e1 = value); break;
	case 7: e2_callback (e2 = value); break;
	case 8: e3_callback (e3 = value); break;
	case 9: e4_callback (e4 = value); break;
	case 10: e5_callback (e5 = value); break;
	case 11: e6_callback (e6 = value); break;
	}
}
void joystick :: axis (int ind, double value) {
	switch (ind) {
	case 0: x_callback (x = value); break;
	case 1: y_callback (y = value); break;
	case 2: z_callback (z = value); break;
	case 3: throttle_callback (throttle = value); break;
	case 4: hx_callback (hx = value); break;
	case 5: hy_callback (hy = value); break;
	default: break;
	}
}
void joystick :: x_callback (double value) {}
void joystick :: y_callback (double value) {}
void joystick :: z_callback (double value) {}
void joystick :: throttle_callback (double value) {}
void joystick :: fire_callback (bool value) {}
void joystick :: autofire_callback (bool value) {}
void joystick :: hx_callback (double value) {}
void joystick :: hy_callback (double value) {}
void joystick :: h1_callback (bool value) {}
void joystick :: h2_callback (bool value) {}
void joystick :: h3_callback (bool value) {}
void joystick :: h4_callback (bool value) {}
void joystick :: e1_callback (bool value) {}
void joystick :: e2_callback (bool value) {}
void joystick :: e3_callback (bool value) {}
void joystick :: e4_callback (bool value) {}
void joystick :: e5_callback (bool value) {}
void joystick :: e6_callback (bool value) {}

bool joystick :: joystick_not_found (void) {return fd < 0;}

joystick :: joystick (char * path) {
	x = y = z = throttle = hx = hy = 0.0;
	fire = autofire = h1 = h2 = h3 = h4 = e1 = e2 = e3 = e4 = e5 = e6 = false;
	fd = open (path, O_RDONLY | O_NONBLOCK);
}
joystick :: ~ joystick (void) {if (fd >= 0) close (fd);}

