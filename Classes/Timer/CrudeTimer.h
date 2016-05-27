#ifndef CRUDETIMER_H
#define CRUDETIMER_H
//------------------------------------------------------------------------
//
//  Name:   CrudeTimer.h
//
//  Desc:   timer to measure time in seconds
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

//this library must be included
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include "../Singleton.h"

#define Clock CrudeTimer::getInstance()

class CrudeTimer : public Singleton<CrudeTimer>
{
public:
	//returns how much time has elapsed since the timer was started
	double getCurrentTime() { return timeGetTime() * 0.001 - _start_time; }

private:
	CrudeTimer(const CrudeTimer&) = delete; // no copies
	CrudeTimer& operator=(const CrudeTimer&) = delete; // no self-assignments
	friend class Singleton<CrudeTimer>;

	//set the start time
	CrudeTimer()
	{
		_start_time = timeGetTime() * 0.001;
	}

	//set to the time (in seconds) when class is instantiated
	double _start_time;
};







#endif