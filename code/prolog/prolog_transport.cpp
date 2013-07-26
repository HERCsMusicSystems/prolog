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

#include "prolog.h"

PrologTransport :: PrologTransport (void) TRACKING (3) {
	miliseconds = 0;
	tick = 0;
	beat = 0;
	bar = 0;
	active = false;
	synchro = true;
	green_synchro = true;
	sub_tick = 0;
	sub_beat = 0;
	ticks_per_beat = 24;
	beats_per_bar = 4;
	delay = 20;
	atempo_beats_per_seconds = beats_per_seconds = 140;
	atempo_seconds = seconds = 60;
	accelerator_current_tick = accelerator_ticks = 1;
	accelerator_step = 0;
	accelerator_sentinel = atempo_beats_per_seconds;
	int dx = 1000 * seconds;
	int dy = beats_per_seconds * 24;//ticks_per_beat;
	d2 = dy + dy;
	d = d2 - dx;
	d1 = d - dx;
}

PrologTransport :: ~ PrologTransport (void) {
	this -> stop ();
}

void PrologTransport :: reset (void) {tick = 0; beat = 0; bar = 0;}

void PrologTransport :: signal (void) {
	tick++;
	if (--sub_tick <= 0) {
		sub_tick = ticks_per_beat;
		beat++;
		if (--sub_beat <= 0) {
			sub_beat = beats_per_bar;
			bar++;
		}
	}
	if (accelerator_step == 0) return;
	accelerator_current_tick--;
	if (accelerator_current_tick > 0) return;
	accelerator_current_tick = accelerator_ticks;
	beats_per_seconds += accelerator_step;
	if (accelerator_step > 0) {
		if (accelerator_sentinel != 0 && beats_per_seconds >= accelerator_sentinel) {
			beats_per_seconds = accelerator_sentinel;
			accelerando ();
		}
	} else {
		if (accelerator_sentinel != 0 && beats_per_seconds <= accelerator_sentinel) {
			beats_per_seconds = accelerator_sentinel;
			accelerando ();
		}
	}
	if (beats_per_seconds < 1) {beats_per_seconds = 1; accelerando ();}
	if (beats_per_seconds / seconds > 100) {beats_per_seconds = 100 * seconds; accelerando ();}
	tempo ();
}

void PrologTransport :: signal_beat (void) {
	while (sub_tick == ticks_per_beat) signal ();
	while (sub_tick < ticks_per_beat) signal ();
}

void PrologTransport :: signal_bar (void) {
	while (sub_beat == beats_per_bar) signal ();
	while (sub_beat < beats_per_bar) signal ();
}

int PrologTransport :: move (int delta) {
	int signals = 0;
	if (active) {
		d += delta * d2;
		while (d >= 0) {
			d += d1;
			signals++;
			signal ();
		}
	} else {
		if (! synchro) {sub_tick = 0; sub_beat = 0;}
		if (green_synchro) synchro = true;
		else green_synchro = true;
	}
	return signals;
}

bool PrologTransport :: green_start (void) {
	if (active) return false;
	active = true;
	if (! synchro) {sub_tick = 0; sub_beat = 0;}
	synchro = true;
	green_synchro = false;
	return true;
}

bool PrologTransport :: stop (void) {
	if (! active) return false;
	synchro = false;
	active = false;
	green_synchro = false;
	return true;
}

bool PrologTransport :: pause (void) {
	if (! active) return false;
	active = false;
	return true;
}

int PrologTransport :: getTick (void) {return tick;}
int PrologTransport :: getBeat (void) {return beat;}
int PrologTransport :: getBar (void) {return bar;}
bool PrologTransport :: isActive (void) {return synchro;}

void PrologTransport :: tempo (void) {
	int dx = 1000 * seconds;
	int dy = beats_per_seconds * 24;//ticks_per_beat;
	d2 = dy + dy;
	int d0 = d2 - dx;
	int dd1 = d0 - dx;
	if (d1 == 0) d1 = dd1;
	d = (int) ((double) d * (double) dd1 / (double) d1);
	d1 = dd1;
}

void PrologTransport :: tempo (int beats_per_minute) {
	atempo_beats_per_seconds = beats_per_seconds = beats_per_minute;
	atempo_seconds = seconds = 60;
	tempo ();
}

void PrologTransport :: tempo (int beats_per_seconds, int seconds) {
	this -> atempo_beats_per_seconds = this -> beats_per_seconds = beats_per_seconds;
	this -> atempo_seconds = this -> seconds = seconds;
	tempo ();
}

void PrologTransport :: accelerando (void) {
	accelerator_step = 0;
	accelerator_ticks = accelerator_current_tick = 1;
	accelerator_sentinel = beats_per_seconds;
}

void PrologTransport :: accelerando (int steps) {
	if (beats_per_seconds + steps < 1) return;
	beats_per_seconds += steps;
	tempo ();
}

void PrologTransport :: accelerando (int steps, int ticks, int sentinel) {
	accelerator_step = steps;
	accelerator_ticks = ticks;
	accelerator_current_tick = 1;
	accelerator_sentinel = sentinel;
	if (accelerator_sentinel > 0) accelerator_sentinel += beats_per_seconds;
	if (accelerator_sentinel < 0) accelerator_sentinel = 1;
}

void PrologTransport :: accelerando (int steps, int ticks) {accelerando (steps, ticks, 0);}

void PrologTransport :: ritardando (void) {accelerando ();}
void PrologTransport :: ritardando (int steps) {accelerando (- steps);}
void PrologTransport :: ritardando (int steps, int ticks) {accelerando (- steps, ticks);}
void PrologTransport :: ritardando (int steps, int ticks, int sentinel) {accelerando (- steps, ticks, - sentinel);}

void PrologTransport :: atempo (void) {accelerando (); tempo (atempo_beats_per_seconds, atempo_seconds);}

void PrologTransport :: division (int beats_per_bar) {
	if (sub_beat > 0) sub_beat += beats_per_bar - this -> beats_per_bar;
	this -> beats_per_bar = beats_per_bar;
}

void PrologTransport :: division (int beats_per_bar, int ticks_per_beat) {
	if (sub_beat > 0) sub_beat += beats_per_bar - this -> beats_per_bar;
	this -> beats_per_bar = beats_per_bar;
	if (sub_tick > 0) sub_tick += ticks_per_beat - this -> ticks_per_beat;
	this -> ticks_per_beat = ticks_per_beat;
}

void PrologTransport :: tick_division (int ticks_per_beat) {
	sub_tick += ticks_per_beat - this -> ticks_per_beat;
	this -> ticks_per_beat = ticks_per_beat;
}

void PrologTransport :: metrum (int top, int bottom) {division (top, 96 / bottom);}

int PrologTransport :: get_beats_per_seconds (void) {return beats_per_seconds;}
int PrologTransport :: get_seconds (void) {return seconds;}
int PrologTransport :: get_beats_per_bar (void) {return beats_per_bar;}
int PrologTransport :: get_ticks_per_beat (void) {return ticks_per_beat;}

