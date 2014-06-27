
#ifndef _LINUX_JOYSTICK_
#define _LINUX_JOYSTICK_


class joystick {
private:
	int fd;
public:
	virtual void axis (int ind, double value);
	virtual void button (int ind, bool value);
	void move (void);
	joystick (char * file_name = "/dev/input/js0");
	~ joystick (void);
};


#endif

