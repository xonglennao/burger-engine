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
#include <SFML/System.hpp>

///	\name	Timer.h
///	\brief	Class used to get the elapsed time between two events
class Timer
{
public:
	/// \brief constructor
	Timer();
	/// \brief destructor;
	~Timer(){}
	
	/// \brief
	void Initialize();

	/// \brief Update the timer
	void Update();

	/// \brief Start the timer
	void Start();
	
	/// \brief Get the elapsed time since the call of sStart();
	float Stop();

	/// \brief Get the elapsed time since last frame
	float GetScaledTime() const {return m_fScaledTime;}

	/// \brief Get the elapsed time since the beginning
	float GetElapsedTime() const {return m_oClock.GetElapsedTime();}

private:
	LARGE_INTEGER m_iStart;
	LARGE_INTEGER m_iFrequency;

	//The sfml clock
	sf::Clock m_oClock;

	//The time since last update
	float m_fScaledTime;

	// The elapsed time from start to last frame
	float m_fLastFrameTime;

};

#endif //__TIMER_H__
