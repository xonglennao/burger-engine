#include "StageTestInput.h"
#include "BurgerEngine/Graphics/DebugDraw.h"
#include "BurgerEngine/Input/EventManager.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"

//MeshTest
#include "BurgerEngine/Graphics/StaticMesh.h"


#include <iostream>

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StageTestInput::StageTestInput(std::string const& a_sId):
	AbstractStage(a_sId),
	m_pTestMesh(NULL),
	m_fFoward(-1.0f),
	m_fStrife(0.0f)
{
	
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::Init()
{
	/// Input Test
	/*Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput01));
	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput02));
	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput03));
		*/
	Engine::GrabInstance().GrabEventManager().RegisterCallbackMousePassiveMotion(
		EventManager::CallbackMouseMotion(this,&StageTestInput::TestInputMouse));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::OnKeyDown));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardUpKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::OnKeyUp));


	/// MeshLoad Test
	m_pTestMesh = new StaticMesh();
	m_pTestMesh->LoadMesh("../Data/Mesh/teapot.obj");

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StageTestInput::~StageTestInput()
{
	/*Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput01));
	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput02));
	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput03));*/

	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::OnKeyDown));

	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardUpKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::OnKeyUp));

	if (m_pTestMesh)
	{
		delete m_pTestMesh;
		m_pTestMesh = NULL;
	}
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::TestInput01(unsigned char a_cKey)
{
	std::cout<<"Input method 1 : "<<a_cKey<<std::endl;

	// Do not do any of those following.
	// You cannot modifiy the EvenManager since you are beeing call by the Vector of callback
	// TODO: a differed Event list, and then you will be able to modify The event manager inside the callback
	//		(and avoid a huge callstack)
	//Engine::GetInstance().GetEventManager().UnRegisterCallbackKeyboardDownKey(
	//EventManager::CallbackKeyboard(this,&StageTestInput::TestInput01));
		
	//Engine::GetInstance().GetEventManager().RegisterCallbackKeyboardDownKey(
	//EventManager::CallbackKeyboard(this,&StageTestInput::TestInput02));

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::TestInput02(unsigned char a_cKey)
{
	std::cout<<"Input method 2 : "<<a_cKey<<" |3 is active, but blocked by 2! (return false)"<<std::endl;
	return false;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::TestInput03(unsigned char a_cKey)
{
	std::cout<<"Input method 3 : "<<a_cKey<<std::endl;
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::OnKeyDown(unsigned char a_cKey)
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
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::OnKeyUp(unsigned char a_cKey)
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

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::TestInputMouse(unsigned int a_uMouseX, unsigned int a_uMouseY)
{
	//std::cout<<"Input method Mouse : "<<a_uMouseX<<" "<<a_uMouseY<<std::endl;
	return true;
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StageTestInput::_Render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Engine const& rEngine = Engine::GetInstance();
	AbstractCamera & rCamera = rEngine.GetCurrentCamera();

	gluLookAt(rCamera.GetPos().x(), rCamera.GetPos().y(), rCamera.GetPos().z(),
		rCamera.GetAim().x(), rCamera.GetAim().y(), rCamera.GetAim().z(),
		rCamera.GetUp().x(), rCamera.GetUp().y(), rCamera.GetUp().z());

	rCamera.Update();

	DebugDraw::DrawAxis(osg::Vec3f(0.0f,0.0f,0.0f),1.0f);
	DebugDraw::DrawGrid(10.0f,20);

	// Render a mesh
	glTranslatef(0.0f,0.0f,50.0f);
	m_pTestMesh->Render();

}