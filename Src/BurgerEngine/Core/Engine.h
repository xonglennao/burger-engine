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

#include "BurgerEngine/Base/Singleton.h"

class EventManager;
class StageManager;
class Window;
class OpenGLContext;
class AbstractCamera;

///	\name	Engine.h
///	\brief	It's the main core of the program
///			it runs all manager etc..
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

	/// \brief	Get the window context
	Window const& GetWindow() const {return *m_pWindow;}

	/// \brief Get Current camera
	/// \todo a camera manager
	AbstractCamera & GetCurrentCamera() const;

	/// \brief	Grabber
	EventManager& GrabEventManager();

	/// \brief	Getter
	EventManager const& GetEventManager() const {return *m_pEventManager;}
	StageManager& GetStageManager() {return *m_pStageManager;}

	/// \brief Setter
	/// A bit dangerous as everyone can call the end...
	void SetTerminate(bool a_bTerminate) {m_bTerminate = a_bTerminate;}
	
protected:

	/// \brief Constructor/Desctructor
	Engine();
	virtual ~Engine(){Terminate();}

private:

	EventManager*	m_pEventManager;
	StageManager*	m_pStageManager;

	/// Current Camera used by the scene
	AbstractCamera* m_pCurrentCamera;

	Window*			m_pWindow;
	OpenGLContext*	m_pRenderingContext;

	///The flag use to exit the running loop;
	bool			m_bTerminate;

};

#endif //__ENGINE_H__
