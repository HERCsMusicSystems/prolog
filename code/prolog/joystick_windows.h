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

#ifndef _WINDOWS_JOYSTICK_
#define _WINDOWS_JOYSTICK_

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class joystick {
public:
	double x, y, z, throttle, hx, hy;
	bool fire, autofire, h1, h2, h3, h4, e1, e2, e3, e4, e5, e6;
	// root callbacks
	virtual void axis (int ind, double value);
	virtual void button (int ind, bool value);
	// surface callbacks
	virtual void x_callback (double value);
	virtual void y_callback (double value);
	virtual void z_callback (double value);
	virtual void throttle_callback (double value);
	virtual void hx_callback (double value);
	virtual void hy_callback (double value);
	virtual void fire_callback (bool value);
	virtual void autofire_callback (bool value);
	virtual void h1_callback (bool value);
	virtual void h2_callback (bool value);
	virtual void h3_callback (bool value);
	virtual void h4_callback (bool value);
	virtual void e1_callback (bool value);
	virtual void e2_callback (bool value);
	virtual void e3_callback (bool value);
	virtual void e4_callback (bool value);
	virtual void e5_callback (bool value);
	virtual void e6_callback (bool value);
	// diagnostics
	virtual bool joystick_not_found (void);
	// move
	void move (double delay = -1.0);
	// constructor / destructor
	joystick (char * path = "/dev/js0");
	~ joystick (void);
public:
	LPDIRECTINPUT8 idi8;
	LPDIRECTINPUTDEVICE8A js;
};


#endif

