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
#include <iostream>
class EventManager;
class StageManager;
class Window;
class OpenGLContext;
class AbstractCamera;
class SceneGraph;
class RenderingContext;

///	\name	Engine.h
///	\brief	It's the main core of the program
///			it runs all manager etc..
class Engine:public Singleton<Engine>
{
	friend class Singleton<Engine>;

public:

	/// \brief Initialize parameters
	void Init( const char* pSceneName );

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
	EventManager&	GrabEventManager();
	SceneGraph&		GrabSceneGraph();
	RenderingContext& GrabRenderContext()  {return *m_pRenderContext;}
	OpenGLContext& GrabRenderingContext(){ return *m_pRenderingContext; }

	/// \brief	Getter
	EventManager const& GetEventManager() const {return *m_pEventManager;}
	StageManager& GetStageManager() {return *m_pStageManager;}
	RenderingContext const& GetRenderContext() const {return *m_pRenderContext;}


	const unsigned int GetWindowWidth() const { return m_iWindowWidth; }
	const unsigned int GetWindowHeight() const { return m_iWindowHeight; }
	void SetViewPort( unsigned int iWidth, unsigned int iHeight ){ m_iWindowWidth = iWidth; m_iWindowHeight = iHeight; }

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

	SceneGraph*		m_pSceneGraph;

	unsigned int	m_iWindowWidth;
	unsigned int	m_iWindowHeight;

	/// The rendering context - Warning the name is close to renderingContext- to change
	RenderingContext* m_pRenderContext;

	///The flag use to exit the running loop;
	bool			m_bTerminate;

};

#endif //__ENGINE_H__
