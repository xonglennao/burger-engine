/*************************************
*
*		BurgerEngine Project
*		
*		Created :	25/09/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#include <Windows.h>

///	\name	Timer.h
///	\brief	Class used to get the elapsed time between two events
class Timer
{
public:
	/// \brief constructor
	Timer();
	/// \brief destructor;
	~Timer(){};
	
	/// \brief Start the timer
	void Start();
	
	/// \brief Get the elapsed time since the call of sStart();
	float Stop();
private:
	LARGE_INTEGER m_iStart;
	LARGE_INTEGER m_iFrequency;

};

#endif //__TIMER_H__
