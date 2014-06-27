
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
	// move
	void move (double delay = -1.0);
	// constructor / destructor
	joystick (char * path);
	~ joystick (void);
public:
	LPDIRECTINPUT8 idi8;
	LPDIRECTINPUTDEVICE8A js;
};


#endif

