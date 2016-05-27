#ifndef REGULATOR
#define REGULATOR
//------------------------------------------------------------------------
//
//  Name:   Regulator.h
//
//  Desc:   Use this class to regulate code flow (for an update function say)
//          Instantiate the class with the frequency you would like your code
//          section to flow (like 10 times per second) and then only allow 
//          the program flow to continue if Ready() returns true
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib") //if you don't use MSVC make sure this library is included in your project
#include <windows.h>
#include "../Util.h"



class Regulator
{
private:

	//the time period between updates 
	double	_update_period;

	//the next time the regulator allows code flow
	DWORD	_next_update_time;


public:


	explicit Regulator(double num_updates_per_second)
	{
		_next_update_time = (DWORD)(timeGetTime());

		if (num_updates_per_second > 0)
		{
			_update_period = 1000.0 / num_updates_per_second;
		}

		else if (util::isEqual(0.0, num_updates_per_second))
		{
			_update_period = 0.0;
		}

		else if (num_updates_per_second < 0)
		{
			_update_period = -1;
		}
	}


	//returns true if the current time exceeds _next_update_time
	bool isReady()
	{
		//if a regulator is instantiated with a zero freq then it goes into
		//stealth mode (doesn't regulate)
		if (util::isEqual(0.0, _update_period)) return true;

		//if the regulator is instantiated with a negative freq then it will
		//never allow the code to flow
		if (_update_period < 0) return false;

		DWORD current_time = timeGetTime();

		//the number of milliseconds the update period can vary per required
		//update-step. This is here to make sure any multiple clients of this class
		//have their updates spread evenly
		static const double update_period_variator = 100.0;

		if (current_time >= _next_update_time)
		{
			_next_update_time = (DWORD)(current_time + _update_period + util::genRand(-update_period_variator, update_period_variator));

			return true;
		}

		return false;
	}
};



#endif