#include "Engine.h"
#include "BurgerEngine/Core/StageManager.h"
#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Core/FirstPersonCamera.h"
#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/TimeContext.h"
#include "BurgerEngine/Core/ObjectFactory.h"

#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/MaterialManager.h"
#include "BurgerEngine/Graphics/TextureManager.h"
#include "BurgerEngine/Graphics/ShaderManager.h"

#include "BurgerEngine/Input/EventManager.h"

#include "BurgerEngine/Graphics/Window.h"
#include "BurgerEngine/Graphics/OpenGLContext.h"
#include "BurgerEngine/Graphics/RenderingContext.h"

#include "BurgerEngine/fx/ParticleContext.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Engine::Engine():
	m_pEventManager(NULL),
	m_pCurrentCamera(NULL),
	m_pRenderingContext(NULL),
	m_pStageManager(NULL),
	m_pRenderContext(NULL),
	m_pParticleContext(NULL),
	m_pTimerContext(NULL)
{}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Init( ObjectFactory* pFactory )
{
	const char * pSceneName = NULL;
	bool bFullScreen;
	
	TiXmlDocument * pDocument = new TiXmlDocument( "../Settings/Settings.xml" );
	if( !pDocument->LoadFile() )
	{
		ADD_ERROR_MESSAGE(" Loading settings file: " << pDocument->ErrorDesc());
	}

	//Get the scene
	TiXmlElement * pRoot = pDocument->FirstChildElement( "settings" );

	if( pRoot )
	{
		TiXmlElement * pScene = pRoot->FirstChildElement( "scene" );
		if( pScene )
		{
			pSceneName = pScene->GetText();
		}
		
		TiXmlElement * pWindow = pRoot->FirstChildElement( "window" );
		if( pWindow )
		{
			int w,h;
			pWindow->QueryIntAttribute("width",&w);
			pWindow->QueryIntAttribute("height",&h);
			m_iWindowWidth = w;
			m_iWindowHeight = h;
			
			std::string sWindowStyle;
			pWindow->QueryValueAttribute<std::string>("style",&sWindowStyle);
			bFullScreen =  (sWindowStyle == "FullScreen");
		}
	}
	
	assert(pSceneName);

	m_pFactory = pFactory;

	m_pTimerContext = new TimerContext();
	m_pTimerContext->Initialize();

	m_pEventManager = new EventManager();
	m_pEventManager->Init();
	
	m_pStageManager = new StageManager();

	m_pWindow = new Window();
	m_pWindow->Initialize( m_iWindowWidth, m_iWindowHeight, bFullScreen );

	//Create the Rendering Context
	m_pRenderingContext = new OpenGLContext();
	m_pRenderingContext->Initialize( m_iWindowWidth, m_iWindowHeight );

	m_pRenderContext = new RenderingContext();
	m_pRenderContext->Initialize();

	m_pParticleContext = new ParticleContext();

	m_pSceneGraph = new SceneGraph( pSceneName );
	
	m_pWindow->GrabDriverWindow().ShowMouseCursor( false );
	
	m_bTerminate = false;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Terminate()
{
	delete m_pParticleContext;
	m_pParticleContext = NULL;

	delete m_pRenderingContext;
	m_pRenderingContext = NULL;

	m_pStageManager->Terminate();
	delete m_pStageManager;
	m_pStageManager = NULL;

	delete m_pCurrentCamera;
	m_pCurrentCamera = NULL;

	delete m_pFactory;
	m_pFactory = NULL;

	MaterialManager::KillInstance();
	MeshManager::KillInstance();
	ShaderManager::KillInstance();
	TextureManager::KillInstance();

	/// Since Event depends on the window, window should be the last to be destroyed, and Event second to last
	m_pEventManager->Clear();
	delete m_pEventManager;
	m_pEventManager = NULL;

	m_pWindow->Terminate();
	delete m_pWindow;
	m_pWindow = NULL;

	delete m_pSceneGraph;
	m_pSceneGraph = NULL;

	delete m_pTimerContext;
	m_pTimerContext = NULL;

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
		m_pTimerContext->Update();

		// Update Particle
		m_pParticleContext->Update();

		//TODO: The main loop might just process event the fact of updating a Scene could be an event.

		//Process Event
		m_pEventManager->ProcessEventList();
		
		float fDeltaTime= m_pTimerContext->GetScaledTime();

		m_pSceneGraph->Update( fDeltaTime, m_pTimerContext->GetElapsedTime() );
		
		m_pStageManager->Update();

		m_pCurrentCamera->Update(fDeltaTime);

		//Render the scene
		m_pRenderContext->Update( fDeltaTime );

		//Swap buffer
		m_pWindow->Display();

		//m_pRenderingContext->CheckError();
	}
}
 

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractCamera & Engine::GetCurrentCamera() const 
{
	// The current Camera can not be null
	assert(m_pCurrentCamera != NULL);
	return *(m_pCurrentCamera);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
EventManager& Engine::GrabEventManager()
{
assert(m_pEventManager != NULL);
return *(m_pEventManager);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SceneGraph& Engine::GrabSceneGraph()
{
	assert(m_pSceneGraph != NULL);
	return *(m_pSceneGraph);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::SetCurrentCamera ( AbstractCamera * pCamera )
{
	if(m_pCurrentCamera)
	{
	delete m_pCurrentCamera;
	}
	m_pCurrentCamera = pCamera;
};
 