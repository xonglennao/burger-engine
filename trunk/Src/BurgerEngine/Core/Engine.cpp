#include "Engine.h"
#include "BurgerEngine/Core/StageManager.h"
#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Core/CameraFps.h"
#include "BurgerEngine/Input/EventManager.h"
#include "BurgerEngine/Graphics/Window.h"
#include "BurgerEngine/Graphics/OpenGLContext.h"

	// I've put this here just to set the same parameters to the window, the camera and the openGL context
	// we might do something a little better than this...
	const float fWindowWidth = 1280.0;
	const float fWindowHeight = 720.0;

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Engine::Engine():
	m_pEventManager(NULL),
	m_pCurrentCamera(NULL),
	m_pRenderingContext(NULL),
	m_pStageManager(NULL)
{}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Init()
{
	m_pEventManager = new EventManager();
	m_pEventManager->Init();
	
	m_pStageManager = new StageManager();

	m_pWindow = new Window();
	m_pWindow->Initialize( fWindowWidth, fWindowHeight );

	// Creation of the camera
	/// \todo this is temporary, we should use a better camera system
	m_pCurrentCamera = new CameraFps();
	m_pCurrentCamera->Initialize( fWindowWidth, fWindowHeight );

	//Create the Rendering Context
	m_pRenderingContext = new OpenGLContext();
	m_pRenderingContext->Initialize( fWindowWidth, fWindowHeight );

	m_bTerminate = false;

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Engine::Terminate()
{
	m_pStageManager->Terminate();
	delete m_pStageManager;
	m_pStageManager = NULL;

	m_pCurrentCamera->Terminate();
	delete m_pCurrentCamera;
	m_pCurrentCamera = NULL;

	/// Since Event depends on the window, window should be the last to be destroyed, and Event second to last
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

		/// \Todo XL: Bad; hard coded and ugly to put this here!
		/// maybe put in into the camera??
		m_pWindow->GrabDriverWindow().SetCursorPosition( fWindowWidth / 2.0, fWindowHeight / 2.0 );
		m_pWindow->GrabDriverWindow().ShowMouseCursor( false );

		//Render Scene
		// \Todo XL: avoid a "rendering" function, better send buffer trough a pipeline
		// and "flush" this pipeline at the end.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //TEMP

		m_pStageManager->Render();

		//Swap buffer
		m_pWindow->Display();

		m_pRenderingContext->CheckError();

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
