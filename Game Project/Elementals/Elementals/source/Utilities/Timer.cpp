#include "Timer.h"

CTimer * CTimer::GetInstance()
{
	// Static local variable
	static CTimer s_Instance;

	// Return the ADDRESS
	return &s_Instance;
}

CTimer::CTimer(void)
{
	time.Signal();
}


CTimer::~CTimer(void)
{
}
