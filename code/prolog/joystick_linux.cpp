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

void joystick :: move (double delay) {}

void joystick :: button (int ind, bool value) {
	switch (ind) {
	case 0: fire_callback (value); break;
	case 1: autofire_callback (value); break;
	case 2: h1_callback (value); break;
	case 3: h2_callback (value); break;
	case 4: h3_callback (value); break;
	case 5: h4_callback (value); break;
	case 6: e1_callback (value); break;
	case 7: e2_callback (value); break;
	case 8: e3_callback (value); break;
	case 9: e4_callback (value); break;
	case 10: e5_callback (value); break;
	case 11: e6_callback (value); break;
	}
}
void joystick :: axis (int ind, double value) {
	switch (ind) {
	case 0: x_callback (value); break;
	case 1: y_callback (value); break;
	case 2: z_callback (value); break;
	case 3: throttle_callback (value); break;
	case 4: hx_callback (value); break;
	case 5: hy_callback (value); break;
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

joystick :: joystick (char * path) {}
joystick :: ~ joystick (void) {}

/*
#include <stdio.h>
#include "joystick_linux.h"

#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>

void joystick :: move (void) {
	struct js_event e;
	while (read (fd, & e, sizeof (e)) > 0) {
		printf ("%4x %2x %2x %2x\n", e . time, e . type, e . number, e . value);
	}
}
*/
/*
void joystick :: move (void) {
	struct js_event e;
	while (read (fd, & e, sizeof (e)) > 0) {
		if (e . type & JS_EVENT_INIT != 0) {
			if (e . type & JS_EVENT_BUTTON != 0) init_button (e . number, e . value != 0);
			if (e . type & JS_EVENT_AXIS != 0) init_axis (e . number, e . value);
		} else {
			if (e . type & JS_EVENT_BUTTON != 0) button (e . number, e . value != 0);
			if (e . type & JS_EVENT_AXIS != 0) axis (e . number, e . value);
		}
	}
}
*/
/*
		if (e . type & JS_EVENT_BUTTON != 0) button (e . number, e . value != 0);
		if (e . type & JS_EVENT_AXIS != 0) axis (e . number, e . value);
	}
}

void joystick :: button (int ind, bool value) {}
void joystick :: axis (int ind, double value) {}

joystick :: joystick (char * file_name) {
	printf ("fd = %i\n", fd);
	fd = open (file_name, O_RDONLY | O_NONBLOCK);
	printf ("open = %i\n", fd);
}

joystick :: ~ joystick (void) {}
*/
