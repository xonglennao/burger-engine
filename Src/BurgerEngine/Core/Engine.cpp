#include "Engine.h"
#include "BurgerEngine/Core/StageManager.h"
#include "BurgerEngine/Input/EventManager.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Init()
{
	m_pEventManager = new EventManager();
	m_pEventManager->Init();
	
	m_pStageManager = new StageManager();
	//m_pStageManager->Init();

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
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Run()
{
	m_bTerminate = true;
	//Let's Roll
	while (m_bTerminate)
	{
	
	}

	
	//exit(0); it's in the main that will quit.
}