#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Core/Timer.h"
#include "BurgerEngine/Graphics/RenderingContext.h"
#include "BurgerEngine/Graphics/DeferredRenderer.h"
#include "BurgerEngine/Input/EventManager.h"
#include "BurgerEngine/GUI/DebugMenu.h"

#include "BurgerEngine/Graphics/Window.h"


#include "BurgerEngineDemo/Stage/MainStage.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
MainStage::MainStage(std::string const& a_sId)
	: AbstractStage(a_sId)
{
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

	rEngine.GetWindow().GrabDriverWindow().SetCursorPosition( iHalfWidth, iHalfHeight );
	
	return true;
}

bool MainStage::OnJoystickMoved(unsigned int a_uStick, float a_fXValue, float a_fYValue)
{
	if(a_uStick == 0 )
	{
		Engine::GetInstance().GetCurrentCamera().SetAnalogY(pow(a_fYValue,3.0f));
		Engine::GetInstance().GetCurrentCamera().SetAnalogX(pow(a_fXValue,3.0f));
	}
	else if(a_uStick == 1 )
	{
		Engine::GetInstance().GetCurrentCamera().UpdateAngles( static_cast<float>(-pow(a_fXValue,3.0f)*50.0f), static_cast<float>(pow(a_fYValue,3.0f)*50.0f) );
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MainStage::Update()
{
}