/*************************************
*
*		BurgerEngine Project
*		
*		Created :	29/06/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __ENGINE_H__
#define __ENGINE_H__

///	\name	Engine.h
///	\brief	It's the main core of the program
///			it runs all manager etc..

#include "BurgerEngine/Base/Singleton.h"

class EventManager;
class StageManager;

class Engine:public Singleton<Engine>
{
	friend class Singleton<Engine>;

public:

	/// \brief Initialize parameters
	void Init();

	/// \brief	Render everything
	void Display();

	/// \brief	Update everything
	void Update();

	/// \brief	Start the Engine loop
	void Run();

	/// \brief	Called to finish eveything
	void Terminate();

	/// \brief	Getter
	inline EventManager& GetEventManager() {return *m_pEventManager;}
	inline StageManager& GetStageManager() {return *m_pStageManager;}

	/// \brief Setter
	/// A bit dangerous as everyone can call the end...
	void SetTerminate(bool a_bTerminate) {m_bTerminate = a_bTerminate;}
	
protected:

	/// \brief Constructor/Desctructor
	Engine(){};
	virtual ~Engine(){Terminate();}

private:

	EventManager*	m_pEventManager;
	StageManager*	m_pStageManager;

	///The flag use to exit the running loop;
	bool			m_bTerminate;

};

#endif //__ENGINE_H__
