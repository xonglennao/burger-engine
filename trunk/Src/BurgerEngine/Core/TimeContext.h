/*************************************
*
*		BurgerEngine Project
*		
*		Created :	31/05/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __TIMECONTEXT_H__
#define __TIMECONTEXT_H__

#include "BurgerEngine/Core/Timer.h"

///	\name	TimerContext.h
///	\brief	Use as a referenceContext for time
class TimerContext
{
public:
	/// \brief constructor
	TimerContext(){}
	/// \brief destructor;
	~TimerContext(){}

	/// \brief
	void Initialize(){m_oTimer.Initialize();}

	/// \brief Update the timer
	void Update(){m_oTimer.Update();}

	/// \brief Get the elapsed time since last frame
	float GetScaledTime() const {return m_oTimer.GetScaledTime();}
	
	/// \brief Get the elapsed time since the begining
	float GetElapsedTime() const {return m_oTimer.GetElapsedTime();}

private:
	//The sfml clock
	Timer m_oTimer;
};

#endif //__TIMECONTEXT_H__