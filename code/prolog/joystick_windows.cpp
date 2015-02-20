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

#include "joystick_windows.h"

static BOOL CALLBACK enumerate_joysticks_callback (LPCDIDEVICEINSTANCE lpd, LPVOID ref) {
	joystick * js = (joystick *) ref;
	if (FAILED (js -> idi8 -> CreateDevice (lpd -> guidInstance, & js -> js, 0))) return DIENUM_CONTINUE;
	if (FAILED (js -> js -> SetDataFormat (& c_dfDIJoystick2))) return DIENUM_CONTINUE;
	if (FAILED (js -> js -> SetCooperativeLevel (FindWindow (0, 0), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) return DIENUM_CONTINUE;
	return DIENUM_STOP;
}

static double process0 (int v) {return (double) ((v + 0x101) >> 9) * 0.015625 - 1.0;}
static double process1 (int v) {return (double) ((v + 0x101) >> 9) * 0.0078125;}

void joystick :: move (double delay) {
	if (js == 0) return;
	if (FAILED (js -> Poll ())) while (DIERR_INPUTLOST == js -> Acquire ()) {}
	DIJOYSTATE2 st;
	if (FAILED (js -> GetDeviceState (sizeof (DIJOYSTATE2), & st))) return;
	double v;
	v = process0 (st . lX); if (x != v) axis (0, x = v);
	v = process0 (st . lY); if (y != v) axis (1, y = v);
	v = process0 (st . lRz); if (z != v) axis (2, z = v);
	v = 2.0 * process1 (st . rglSlider [0]) - 1.0; if (throttle != v) axis (3, throttle = v);
	double hhx = 0.0, hhy = 0.0;
	switch (st . rgdwPOV [0]) {
	case 0: hhy = -1.0; break;
	case 4500: hhy = -1.0; hhx = 1.0; break;
	case 9000: hhx = 1.0; break;
	case 13500: hhy = hhx = 1.0; break;
	case 18000: hhy = 1.0; break;
	case 22500: hhy = 1.0; hhx = -1.0; break;
	case 27000: hhx = -1.0; break;
	case 31500: hhy = hhx = -1.0; break;
	default: break;
	}
	if (hx != hhx) axis (4, hx = hhx);
	if (hy != hhy) axis (5, hy = hhy);
	bool b;
	b = st . rgbButtons [0] != 0; if (fire != b) button (0, fire = b);
	b = st . rgbButtons [1] != 0; if (autofire != b) button (1, autofire = b);
	b = st . rgbButtons [2] != 0; if (h1 != b) button (2, h1 = b);
	b = st . rgbButtons [3] != 0; if (h2 != b) button (3, h2 = b);
	b = st . rgbButtons [4] != 0; if (h3 != b) button (4, h3 = b);
	b = st . rgbButtons [5] != 0; if (h4 != b) button (5, h4 = b);
	b = st . rgbButtons [6] != 0; if (e1 != b) button (6, e1 = b);
	b = st . rgbButtons [7] != 0; if (e2 != b) button (7, e2 = b);
	b = st . rgbButtons [8] != 0; if (e3 != b) button (8, e3 = b);
	b = st . rgbButtons [9] != 0; if (e4 != b) button (9, e4 = b);
	b = st . rgbButtons [10] != 0; if (e5 != b) button (10, e5 = b);
	b = st . rgbButtons [11] != 0; if (e6 != b) button (11, e6 = b);
	if (delay < 0.0) return;
	if (delay == 0.0) Sleep (10);
	else Sleep ((int) delay);
}

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

bool joystick :: joystick_not_found (void) {return js == 0;}

joystick :: joystick (char * path) {
	idi8 = 0; js = 0;
	x = y = z = throttle = hx = hy = 0.0;
	fire = autofire = h1 = h2 = h3 = h4 = e1 = e2 = e3 = e4 = e5 = e6 = false;
	if (FAILED (DirectInput8Create (GetModuleHandle (0), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *) & idi8, 0))) return;
	if (idi8 == 0) return;
	idi8 -> EnumDevices (DI8DEVCLASS_GAMECTRL, enumerate_joysticks_callback, this, DIEDFL_ATTACHEDONLY);
}

joystick :: ~ joystick (void) {}

