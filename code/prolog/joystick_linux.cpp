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

