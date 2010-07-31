#include "Engine.h"
#include "BurgerEngine/Core/StageManager.h"
#include "BurgerEngine/Input/EventManager.h"
#include "BurgerEngine/Graphics/Window.h"
#include "BurgerEngine/Graphics/OpenGLContext.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Init()
{
	m_pEventManager = new EventManager();
	m_pEventManager->Init();
	
	m_pStageManager = new StageManager();

	m_pWindow = new Window();
	m_pWindow->Init();

	//Create the Rendering Context
	m_pRenderingContext = new OpenGLContext();
	m_pRenderingContext->Init();

	m_bTerminate = false;

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Terminate()
{
	m_pStageManager->Clear();
	delete m_pStageManager;
	m_pStageManager = NULL;

	m_pEventManager->Clear();
	delete m_pEventManager;
	m_pEventManager = NULL;

	m_pWindow->Terminate();
	delete m_pWindow;
	m_pWindow = NULL;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Run()
{
	m_bTerminate = false;
	//Let's Roll
	while (m_bTerminate == false)
	{

		//Update Scene
		//TODO: The main loop might just process event the fact of updating a Scene could be an event.
		m_pStageManager->Update();

		//Process Event
		m_pEventManager->ProcessEventList();

		//Render Scene
		// \Todo: avoid a "rendering" function, better send buffer trough a pipeline
		// and "flush" this pipeline at the end.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //TEMP

		m_pStageManager->Render();

		//Swap buffer
		m_pWindow->Display();

		m_pRenderingContext->CheckError();

	}

}