///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2013 Robert P. Wolf                       //
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

class WaitFor;

class PrologTransport TRACK {
private:
	WaitFor * waiters;
	pthread_mutex_t lock;
	friend void * transportRunner (void * parameter);
	bool processRunning, processStopping, processPaused;
	int tick, beat, bar;
	int sub_tick, sub_beat;
	int ticks_per_beat, beats_per_bar;
	double delay;
	double beats_per_minute;
	int beats_per_seconds, beat_seconds;
	void broadcast (WaitFor * * waiter);
	void broadcastStop (void);
public:
	PrologTransport (void);
	~ PrologTransport (void);
	void reset (void);
	void reset (int beats);
	void reset (int beats, int ticks);
	void reset (double beats);
	void signal (void);
	void signalBeat (void);
	void signalBar (void);
	bool start (void);
	bool stop (void);
	bool pause (void);
	bool wait (int ticks = 1);
	bool waitBeat (int beats = 1);
	bool waitBeat (double beats);
	bool waitBar (int bars = 1);
	int getTick (void);
	int getBeat (void);
	int getBar (void);
	bool isActive (void);
	void tempo (void);
	void tempo (double beatsPerMinute);
	void tempo (int beats, int seconds);
	void accelerando (void);
	void accelerando (int steps);
	void accelerando (int steps, int ticks);
	void accelerando (int steps, int ticks, int sentinel);
	void ritardando (void);
	void ritardando (int steps);
	void ritardando (int steps, int ticks);
	void ritardando (int steps, int ticks, int sentinel);
	void atempo (void);
	void division (int beatsPerBar);
	void division (int beatsPerBar, int ticksPerBeat);
	void tickDivision (int ticksPerBeat);
	void metrum (int top, int bottom);
	double getBeatsPerMinute (void);
	int getBeatsPerBar (void);
	int getTicksPerBeat (void);
	int getBeatsPerSeconds (void);
	int getBeatSeconds (void);
};

