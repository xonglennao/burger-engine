#include "Engine.h"
#include "BurgerEngine/Core/StageManager.h"
#include "BurgerEngine/Input/EventManager.h"
#include "BurgerEngine/Graphics/Window.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Init()
{
	m_pEventManager = new EventManager();
	m_pEventManager->Init();
	
	m_pStageManager = new StageManager();
	//m_pStageManager->Init();

	m_pWindow = new Window();
	m_pWindow->Init();

	m_bTerminate = false;

	//Create the window Context
	//.... bla bla bla
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
		m_pEventManager->ProcessEventList();
	}

	
	//exit(0); it's in the main that will quit.
}