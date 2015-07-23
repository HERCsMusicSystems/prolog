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

#ifdef WIN32
#define WINDOWS_OPERATING_SYSTEM
#else
#define LINUX_OPERATING_SYSTEM
#endif
//#define MAC_OPERATING_SYSTEM

#define MULTICORE_MULTIPROCESSING


#ifdef WINDOWS_OPERATING_SYSTEM
#ifdef SDK_IMPORT
#define API __declspec (dllimport)
#elif SDK_EXPORT
#define API __declspec (dllexport)
#else
#define API
#endif
#endif

#ifdef LINUX_OPERATING_SYSTEM
#define API
#endif

#ifdef MULTICORE_MULTIPROCESSING

#ifdef WINDOWS_OPERATING_SYSTEM

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#endif
#ifndef WINVER
#define WINVER 0x501
#endif
#include <windows.h>
#include <winbase.h>
#include <pthread.h>

#define COLLECTOR_RESET(index) reference_counter = index; InitializeCriticalSectionAndSpinCount (& critical_section, 0x4000);
#define COLLECTOR int reference_counter; CRITICAL_SECTION critical_section;\
	bool dec (void) {EnterCriticalSection (& critical_section); bool ret = --reference_counter == 0; LeaveCriticalSection (& critical_section); return ret;}\
	void inc (void) {EnterCriticalSection (& critical_section); reference_counter++; LeaveCriticalSection (& critical_section);}
#define NOT_COLLECTOR_DEC ! dec ()
#define COLLECTOR_DEC dec ()
#define COLLECTOR_REFERENCE_INC(atom) atom -> inc ();
#define COLLECTOR_DESTROY DeleteCriticalSection (& critical_section);
#define TRACKER_DEFINITIONS\
	static CRITICAL_SECTION tracker_critical_section;\
	static BOOL tracker_critical_section_initialization_result = InitializeCriticalSectionAndSpinCount (& tracker_critical_section, 0x4000);
#define TRACKER_ENTER EnterCriticalSection (& tracker_critical_section)
#define TRACKER_LEAVE LeaveCriticalSection (& tracker_critical_section)

#endif

#ifdef LINUX_OPERATING_SYSTEM
#include <pthread.h>
#define COLLECTOR_RESET(index)\
	reference_counter = index;\
	pthread_mutexattr_t mutex_attr;\
	pthread_mutexattr_init (& mutex_attr);\
	pthread_mutex_init (& critical_section, & mutex_attr);\
	pthread_mutexattr_destroy (& mutex_attr);
//critical_section = PTHREAD_MUTEX_INITIALIZER;
#define COLLECTOR int reference_counter; pthread_mutex_t critical_section;\
	bool dec (void) {pthread_mutex_lock (& critical_section); bool ret = --reference_counter == 0; pthread_mutex_unlock (& critical_section); return ret;}\
	void inc (void) {pthread_mutex_lock (& critical_section); reference_counter++; pthread_mutex_unlock (& critical_section);}
#define NOT_COLLECTOR_DEC ! dec ()
#define COLLECTOR_DEC dec ()
#define COLLECTOR_REFERENCE_INC(atom) atom -> inc ();
#define COLLECTOR_DESTROY pthread_mutex_destroy (& critical_section);
#define TRACKER_DEFINITIONS	static pthread_mutex_t tracker_critical_section = PTHREAD_MUTEX_INITIALIZER;
#define TRACKER_ENTER pthread_mutex_lock (& tracker_critical_section)
#define TRACKER_LEAVE pthread_mutex_unlock (& tracker_critical_section)

#endif

#else

#define COLLECTOR_RESET(index) reference_counter = index;
#define COLLECTOR int reference_counter;
#define NOT_COLLECTOR_DEC --reference_counter != 0
#define COLLECTOR_DEC --reference_counter == 0
#define COLLECTOR_REFERENCE_INC(atom) atom -> reference_counter++;
#define COLLECTOR_DESTROY
#define TRACKER_DEFINITIONS
#define TRACKER_ENTER
#define TRACKER_LEAVE

#endif

#ifndef NO_TRACKING

class API object_tracker {
private:
	int type;
public:
	object_tracker (int selector);
	~ object_tracker (void);
};

#define TRACK : public object_tracker
#define TRACKING(index) : object_tracker (index)

#else

#define TRACK
#define TRACKING(index)

#endif

