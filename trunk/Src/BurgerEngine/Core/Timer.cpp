#include "Timer.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Timer::Timer()
{
	QueryPerformanceFrequency(&m_iFrequency);
	m_iStart.QuadPart = (LONGLONG)0.0;
}

void Timer::Start()
{
	QueryPerformanceCounter(&m_iStart);
}

float Timer::Stop()
{
	LARGE_INTEGER iStop;
	QueryPerformanceCounter(&iStop);
	LONGLONG iTimeDiff = iStop.QuadPart - m_iStart.QuadPart;
	//returns duration in milliseconds
	return (float) iTimeDiff * 1000.0f / (float) m_iFrequency.QuadPart;
}
