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
#include "prolog_transport.h"

#ifdef WINDOWS_OPERATING_SYSTEM
#define usleep(delay) Sleep(delay / 1000);
#endif

#ifdef LINUX_OPERATING_SYSTEM
#include <unistd.h>
#endif

void * transportRunner (void * parameters) {
	PrologTransport * transport = (PrologTransport *) parameters;
	while (transport -> processRunning) {
		if (! transport -> processPaused) transport -> signal ();
		usleep ((int) transport -> delay);
	}
	transport -> broadcastStop ();
	transport -> processRunning = true;
	return 0;
}

enum WaitForType {TICK = 1, BEAT, BAR};

class WaitFor {
public:
	enum WaitForType type;
	int sentinel;
	pthread_cond_t conditional;
	WaitFor * next;
	WaitFor (enum WaitForType type, int sentinel, WaitFor * next = 0) {
		this -> type = type; this -> sentinel = sentinel; this -> next = next;
		conditional = PTHREAD_COND_INITIALIZER;
	}
	~ WaitFor (void) {pthread_cond_destroy (& conditional);}
};

void PrologTransport :: broadcast (WaitFor * * waiter) {
	while (* waiter != 0) {
		int threshold = (* waiter) -> type == WaitForType :: BAR ? bar : (* waiter) -> type == WaitForType :: BEAT ? beat : tick;
		if (threshold >= (* waiter) -> sentinel) {
			pthread_cond_signal (& (* waiter) -> conditional);
			WaitFor * p = * waiter;
			* waiter = p -> next;
			delete p;
		} else waiter = & (* waiter) -> next;
	}
}

void PrologTransport :: broadcastStop (void) {
	pthread_mutex_lock (& lock);
	processStopping = true;
	while (waiters != 0) {
		pthread_cond_signal (& waiters -> conditional);
		WaitFor * p = waiters -> next;
		delete waiters;
		waiters = p;
	}
	pthread_mutex_unlock (& lock);
}

bool PrologTransport :: wait (int ticks) {
	pthread_mutex_lock (& lock);
	waiters = new WaitFor (TICK, tick + ticks, waiters);
	pthread_cond_wait (& waiters -> conditional, & lock);
	bool ret = processStopping;
	pthread_mutex_unlock (& lock);
	return ! ret;
}

bool PrologTransport :: waitBeat (int beats) {
	pthread_mutex_lock (& lock);
	waiters = new WaitFor (WaitForType :: BEAT, beat + beats, waiters);
	pthread_cond_wait (& waiters -> conditional, & lock);
	bool ret = processStopping;
	pthread_mutex_unlock (& lock);
	return ! ret;
}

bool PrologTransport :: waitBeat (double beats) {
	int integral = (int) beats;
	double fractional = beats - (double) integral;
	if (! waitBeat (integral)) return false;
	return wait ((int) (fractional * (double) ticks_per_beat));
}

bool PrologTransport :: waitBar (int bars) {
	pthread_mutex_lock (& lock);
	waiters = new WaitFor (WaitForType :: BAR, bar + bars, waiters);
	pthread_cond_wait (& waiters -> conditional, & lock);
	bool ret = processStopping;
	pthread_mutex_unlock (& lock);
	return ! ret;
}

PrologTransport :: PrologTransport (void) TRACKING (3) {
	waiters = 0;
	processRunning = processPaused = processStopping = false;
	metrum (4, 4);
	reset ();
	tempo (140.0);
	lock = PTHREAD_MUTEX_INITIALIZER;
}

PrologTransport :: ~ PrologTransport (void) {stop (); pthread_mutex_destroy (& lock);}

void PrologTransport :: reset (void) {sub_tick = tick = 0; sub_beat = beat = 0; bar = 0;}
void PrologTransport :: reset (int beats) {tick = 0; beat = 0; sub_beat = beats; bar = 0;}
void PrologTransport :: reset (int beats, int ticks) {tick = 0; sub_tick = ticks; beat = 0; sub_beat = beats; bar = 0;}
void PrologTransport :: reset (double beats) {tick = 0; sub_tick = (int) ((beats - (double) ((int) beats)) * (double) ticks_per_beat); beat = 0; sub_beat = (int) beats; bar = 0;}

bool PrologTransport :: start (void) {
	pthread_mutex_lock (& lock);
	if (processPaused) {processPaused = false; pthread_mutex_unlock (& lock); return true;}
	if (processRunning) {pthread_mutex_unlock (& lock); return false;}
	processRunning = true;
	processPaused = processStopping = false;
	pthread_t thread;
	pthread_create (& thread, 0, transportRunner, this);
	pthread_detach (thread);
	pthread_mutex_unlock (& lock);
	return true;
}

bool PrologTransport :: pause (void) {
	pthread_mutex_lock (& lock);
	if (processPaused || ! processRunning) {pthread_mutex_unlock (& lock); return false;}
	processPaused = true;
	pthread_mutex_unlock (& lock);
	return true;
}

bool PrologTransport :: stop (void) {
	pthread_mutex_lock (& lock);
	if (! processRunning) {pthread_mutex_unlock (& lock); return false;}
	processRunning = false;
	pthread_mutex_unlock (& lock);
	while (! processRunning) {usleep (10000);}
	processRunning = false;
	return true;
}

void PrologTransport :: signal (void) {
	pthread_mutex_lock (& lock);
	tick++;
	if (--sub_tick <= 0) {
		sub_tick = ticks_per_beat; beat++;
		if (--sub_beat <= 0) {sub_beat = beats_per_bar; bar++;}
	}
	broadcast (& waiters);
	pthread_mutex_unlock (& lock);
}

void PrologTransport :: signalBeat (void) {
	pthread_mutex_lock (& lock);
	beat++;
	if (--sub_beat <= 0) {sub_beat = beats_per_bar; bar++;}
	broadcast (& waiters);
	pthread_mutex_unlock (& lock);
}

void PrologTransport :: signalBar (void) {
	pthread_mutex_lock (& lock);
	bar++;
	broadcast (& waiters);
	pthread_mutex_unlock (& lock);
}

int PrologTransport :: getTick (void) {return tick;}
int PrologTransport :: getBeat (void) {return beat;}
int PrologTransport :: getBar (void) {return bar;}
int PrologTransport :: getBeatsPerBar (void) {return beats_per_bar;}
int PrologTransport :: getTicksPerBeat (void) {return ticks_per_beat;}
int PrologTransport :: getBeatsPerSeconds (void) {return beats_per_seconds;}
int PrologTransport :: getBeatSeconds (void) {return beat_seconds;}
double PrologTransport :: getBeatsPerMinute (void) {return beats_per_minute;}
void PrologTransport :: tempo (void) {
	delay = 60000000.0 / (beats_per_minute * (double) ticks_per_beat);
}
void PrologTransport :: tempo (double beatsPerMinute) {
	if (beatsPerMinute <= 0.0) beatsPerMinute = 1.0;
	beats_per_minute = beatsPerMinute; beats_per_seconds = (int) beatsPerMinute; beat_seconds = 60;
	tempo ();
}
void PrologTransport :: tempo (int beats, int seconds) {
	if (beats <= 0) beats = 1;
	if (seconds <= 0) seconds = 1;
	beats_per_seconds = beats; beat_seconds = seconds;
	beats_per_minute = (double) beats * 60.0 / (double) seconds;
	tempo ();
}
void PrologTransport :: division (int beatsPerBar) {
	if (sub_beat > 0) sub_beat += beatsPerBar - beats_per_bar;
	beats_per_bar = beatsPerBar;
}
void PrologTransport :: division (int beatsPerBar, int ticksPerBeat) {
	if (sub_beat > 0) sub_beat += beatsPerBar - beats_per_bar;
	beats_per_bar = beatsPerBar;
	if (sub_tick > 0) sub_tick += ticksPerBeat - ticks_per_beat;
	ticks_per_beat = ticksPerBeat;
}
void PrologTransport :: tickDivision (int ticksPerBeat) {
	if (sub_tick > 0) sub_tick += ticksPerBeat - ticks_per_beat;
	ticks_per_beat = ticksPerBeat;
}
void PrologTransport :: metrum (int top, int bottom) {if (bottom > 0) division (top, 96 / bottom);}

void PrologTransport :: accelerando (void) {}
void PrologTransport :: accelerando (int steps) {}
void PrologTransport :: accelerando (int steps, int ticks) {}
void PrologTransport :: accelerando (int steps, int ticks, int sentinel) {}
void PrologTransport :: atempo (void) {tempo ();}

void PrologTransport :: ritardando (void) {accelerando ();}
void PrologTransport :: ritardando (int steps) {accelerando (- steps);}
void PrologTransport :: ritardando (int steps, int ticks) {accelerando (- steps, ticks);}
void PrologTransport :: ritardando (int steps, int ticks, int sentinel) {accelerando (- steps, ticks, - sentinel);}

/*
PrologTransport transport;

void * ticker (void * parameter) {
	while (transport . wait ()) {
		printf ("%s\n", (char *) parameter);
	}
	printf ("ticker stopped\n");
	return 0;
}

void * beater (void * parameter) {
	while (true) {
		transport . waitBeat ();
		printf ("%s\n", (char *) parameter);
	}
}

void * barrer (void * parameter) {
	while (true) {
		transport . waitBar ();
		printf ("%s\n", (char *) parameter);
	}
}

int main (int args, char * * argv) {
	pthread_t tck1; pthread_create (& tck1, 0, ticker, (void *) "    tick"); pthread_detach (tck1);
	pthread_t be1; pthread_create (& be1, 0, beater, (void *) "      beat"); pthread_detach (be1);
	pthread_t bar1; pthread_create (& bar1, 0, barrer, (void *) "        bar"); pthread_detach (bar1);
	char x = getchar ();
	while (x != 'x') {
		switch (x) {
		case 'b': printf (transport . start () ? "started\n" : "failed\n"); break;
		case 'n': printf (transport . pause () ? "paused\n" : "failed\n"); break;
		case 'm': printf (transport . stop () ? "stopped\n" : "failed\n"); break;
		default: break;
		}
		x = getchar ();
	}
	return 0;
}
*/

/*
PrologTransport :: PrologTransport (void) TRACKING (3) {
	printf ("creating transport.....\n");
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
	printf ("transport created....\n");
}

PrologTransport :: ~ PrologTransport (void) {
	this -> stop ();
}

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

*/
