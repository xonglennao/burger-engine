#include "BurgerEngine/Core/StageMain3DScene.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Core/Timer.h"

#include "BurgerEngine/Input/EventManager.h"

#include "BurgerEngine/Graphics/DeferredRenderer.h"

//#include <iostream>

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StageMain3DScene::StageMain3DScene(std::string const& a_sId)
	: AbstractStage(a_sId)
{
	m_pDeferredRenderer = new DeferredRenderer();
	m_oTimer = new Timer();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageMain3DScene::Init()
{
	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageMain3DScene::OnKeyDown));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardUpKey(
		EventManager::CallbackKeyboard(this,&StageMain3DScene::OnKeyUp));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackMousePassiveMotion(
		EventManager::CallbackMouseMotion(this,&StageMain3DScene::OnMouseMoved));

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StageMain3DScene::~StageMain3DScene()
{
	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageMain3DScene::OnKeyDown));

	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardUpKey(
		EventManager::CallbackKeyboard(this,&StageMain3DScene::OnKeyUp));

	delete m_pDeferredRenderer;
	m_pDeferredRenderer = NULL;

	delete m_oTimer;
	m_oTimer = NULL;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageMain3DScene::OnKeyDown(unsigned char a_cKey)
{
	
	AbstractCamera::CameraFlagEnum e = AbstractCamera::E_CAMERA_LEFT;
	
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
		case 40 :
		case 'o':
			m_pDeferredRenderer->SetDebugFlag(0);
			break;
		case 41 :
		case 'p':
			m_pDeferredRenderer->SetDebugFlag(1);
			break;
		case 42 :
			m_pDeferredRenderer->SetDebugFlag(2);
			break;
		case 43 :
			m_pDeferredRenderer->SetDebugFlag(3);
			break;
		case 44 :
			m_pDeferredRenderer->SetDebugFlag(4);
			break;
		break;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageMain3DScene::OnKeyUp(unsigned char a_cKey)
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
	}
	return true;
}

bool StageMain3DScene::OnMouseMoved(unsigned int a_uX, unsigned int a_uY)
{
	Engine const& rEngine = Engine::GetInstance();
	
	float fAlpha = (rEngine.GetWindowWidth() / 2.0f - static_cast<float>(a_uX) );
	float fPhi = ( rEngine.GetWindowHeight() /2.0f - static_cast<float>(a_uY) );

	Engine::GetInstance().GetCurrentCamera().UpdateAngles( fAlpha, fPhi );

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StageMain3DScene::_Render()
{
	float fDeltaTime = m_oTimer->Stop();
	m_oTimer->Start();
	Engine::GetInstance().GetCurrentCamera().Update( fDeltaTime );
	m_pDeferredRenderer->Render();


}