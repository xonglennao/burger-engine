#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Core/FirstPersonCamera.h"
#include "BurgerEngine/Core/Timer.h"
#include "BurgerEngine/Graphics/RenderingContext.h"
#include "BurgerEngine/Graphics/DeferredRenderer.h"
#include "BurgerEngine/GUI/DebugMenu.h"

#include "BurgerEngine/Graphics/Window.h"


#include "BurgerEngineDemo/Stage/MainStage.h"
#include "BurgerEngineDemo/Manager/GameplayManager.h"
#include "BurgerEngineDemo/Component/PlayerComponent.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
MainStage::MainStage(std::string const& a_sId)
	: AbstractStage(a_sId)
	, m_bFreeCam(false)
{
	//freecam hack
	m_pPlayerCamera = Engine::GetInstance().GetCurrentCameraPointer();
	m_pFreeCamera = new FirstPersonCamera(45.0f,vec3(0.0f, 10.0f,0.0f), vec2(0.0f,0.0f), vec4(-150.0f, 30.0f, 155.0f,1.0f), vec2(30.0f,0.05f));
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool MainStage::Init()
{
	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
	EventManager::CallbackKeyboard(this,&MainStage::OnKeyDown));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardUpKey(
	EventManager::CallbackKeyboard(this,&MainStage::OnKeyUp));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackMousePassiveMotion(
	EventManager::CallbackMouseMotion(this,&MainStage::OnMouseMoved));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackJoystick(
		EventManager::CallbackXBoxJoystick(this,&MainStage::OnJoystickMoved));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackPadButton(
		EventManager::CallbackPadButtonPressed(this,&MainStage::OnPadButtonPressed));

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
MainStage::~MainStage()
{
	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&MainStage::OnKeyDown));

	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardUpKey(
		EventManager::CallbackKeyboard(this,&MainStage::OnKeyUp));

	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackJoystick(
		EventManager::CallbackXBoxJoystick(this,&MainStage::OnJoystickMoved));


	delete m_pPlayerCamera;
	m_pPlayerCamera = NULL;

	delete m_pFreeCamera;
	m_pFreeCamera = NULL;

	Engine::GrabInstance().SetCurrentCamera( NULL );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool MainStage::OnKeyDown(unsigned char a_cKey)
{
	switch ( a_cKey )
	{
		case 35 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_LEFT, true );
		break;
		case 36 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_RIGHT, true );
		break;
		case 37 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_FORWARD, true );
		break;
		case 38 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_BACKWARD, true );
		break;
		case 25 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_DOF_NEAR_FORWARD, true );
		break;
		case 26 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_DOF_NEAR_BACKWARD, true );
		break;
		case 27 :
			if( Engine::GrabInstance().GrabRenderContext().GrabRenderer().GetShowDebugMenu() )
				Engine::GrabInstance().GrabRenderContext().GetDebugMenu().SetFlag( DebugMenu::E_PREVIOUS, true );
		break;
		case 28 :
			if( Engine::GrabInstance().GrabRenderContext().GrabRenderer().GetShowDebugMenu() )
				Engine::GrabInstance().GrabRenderContext().GetDebugMenu().SetFlag( DebugMenu::E_NEXT, true );
		break;
		case 31 :
			if( Engine::GrabInstance().GrabRenderContext().GrabRenderer().GetShowDebugMenu() )
				Engine::GrabInstance().GrabRenderContext().GetDebugMenu().SetFlag( DebugMenu::E_INCREMENT, true );
		break;
		case 32 :
			if( Engine::GrabInstance().GrabRenderContext().GrabRenderer().GetShowDebugMenu())
				Engine::GrabInstance().GrabRenderContext().GetDebugMenu().SetFlag( DebugMenu::E_DECREMENT, true );
		break;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool MainStage::OnKeyUp(unsigned char a_cKey)
{
	switch ( a_cKey )
	{
		case 35 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_LEFT, false );
		break;
		case 36 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_RIGHT, false );
		break;
		case 37 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_FORWARD, false );
		break;
		case 38 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_BACKWARD, false );
		break;
		case 25 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_DOF_NEAR_FORWARD, false );
		break;
		case 26 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_DOF_NEAR_BACKWARD, false );
		break;
		case 27 :
			if( Engine::GrabInstance().GrabRenderContext().GrabRenderer().GetShowDebugMenu() )
				Engine::GrabInstance().GrabRenderContext().GetDebugMenu().SetFlag( DebugMenu::E_PREVIOUS, false );
		break;
		case 28 :
			if( Engine::GrabInstance().GrabRenderContext().GrabRenderer().GetShowDebugMenu() )
				Engine::GrabInstance().GrabRenderContext().GetDebugMenu().SetFlag( DebugMenu::E_NEXT, false );
		break;
		case 31 :
			if( Engine::GrabInstance().GrabRenderContext().GrabRenderer().GetShowDebugMenu() )
				Engine::GrabInstance().GrabRenderContext().GetDebugMenu().SetFlag( DebugMenu::E_INCREMENT, false );
		break;
		case 32 :
			if( Engine::GrabInstance().GrabRenderContext().GrabRenderer().GetShowDebugMenu() )
				Engine::GrabInstance().GrabRenderContext().GetDebugMenu().SetFlag( DebugMenu::E_DECREMENT, false );
		break;
		case 21 :
			Engine::GrabInstance().GrabRenderContext().GrabRenderer().SwitchShowDebugMenu();
		break;
	}
	return true;
}

bool MainStage::OnMouseMoved(unsigned int a_uX, unsigned int a_uY)
{
	Engine const& rEngine = Engine::GetInstance();
	
	unsigned int iHalfWidth = rEngine.GetWindowWidth() / 2;
	unsigned int iHalfHeight = rEngine.GetWindowHeight() / 2;

	int iAlpha = ( iHalfWidth - a_uX );
	int iPhi = ( iHalfHeight - a_uY );
	
	Engine::GetInstance().GetCurrentCamera().UpdateAngles( static_cast<float>(iAlpha), static_cast<float>(iPhi) );
	
	PlayerComponent * pCurrentPlayer = pCurrentPlayer = GameplayManager::GrabInstance().GetCurrentPlayer();
	if(pCurrentPlayer)
	{
		pCurrentPlayer->UpdateAngles( static_cast<float>(-iAlpha), static_cast<float>(-iPhi) );
	}
	
	rEngine.GetWindow().GrabDriverWindow().SetCursorPosition( iHalfWidth, iHalfHeight );
	
	return true;
}

bool MainStage::OnJoystickMoved(unsigned int a_uStick, float a_fXValue, float a_fYValue)
{
	PlayerComponent * pCurrentPlayer = pCurrentPlayer = GameplayManager::GrabInstance().GetCurrentPlayer();

	if(a_uStick == 0 )
	{
		if(pCurrentPlayer && !m_bFreeCam)
		{
			pCurrentPlayer->SetAnalogX(a_fXValue);
			pCurrentPlayer->SetAnalogY(a_fYValue);
		}
		else if(m_bFreeCam)
		{
			Engine::GetInstance().GetCurrentCamera().SetAnalogX(a_fXValue);
			Engine::GetInstance().GetCurrentCamera().SetAnalogY(a_fYValue);

			if(pCurrentPlayer)
			{
				pCurrentPlayer->SetAnalogX(0.0f);
				pCurrentPlayer->SetAnalogY(0.0f);
			}
		}
	}
	else if(a_uStick == 1 )
	{
		Engine::GetInstance().GetCurrentCamera().UpdateAngles( static_cast<float>(-pow(a_fXValue,3.0f)*50.0f), static_cast<float>(pow(a_fYValue,3.0f)*50.0f) );
		
		if(pCurrentPlayer && !m_bFreeCam)
		{
			pCurrentPlayer->UpdateAngles( static_cast<float>(pow(a_fXValue,3.0f)*50.0f), static_cast<float>(-pow(a_fYValue,3.0f)*50.0f) );
		}
	}
	return true;
}

bool MainStage::OnPadButtonPressed(EventManager::PAD_BUTTON iButton, bool bPressed)
{
	PlayerComponent * pCurrentPlayer = pCurrentPlayer = GameplayManager::GrabInstance().GetCurrentPlayer();

	switch(iButton)
	{
		case EventManager::PAD_BUTTON_1 :
		{
			if(pCurrentPlayer)
			{
			}
			break;
		}
		
		case EventManager::PAD_BUTTON_L :
		{
			if(bPressed)
			{
				m_bFreeCam = !m_bFreeCam;

				if(m_bFreeCam)
				{
					Engine::GrabInstance().SetCurrentCamera( m_pFreeCamera );
				}
				else
				{
					Engine::GrabInstance().SetCurrentCamera( m_pPlayerCamera );
				}
			}
			break;
		}
	}
	return true;
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MainStage::Update()
{
	PlayerComponent * pCurrentPlayer = pCurrentPlayer = GameplayManager::GrabInstance().GetCurrentPlayer();

	if(pCurrentPlayer && !m_bFreeCam)
	{
		Engine::GetInstance().GetCurrentCamera().SetPosition( pCurrentPlayer->GetPos() );
	}
}