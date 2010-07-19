#include "StageTestInput.h"
#include "BurgerEngine/Input/EventManager.h"
#include "BurgerEngine/Core/Engine.h"

//MeshTest
#include "BurgerEngine/Graphics/StaticMesh.h"
#include <iostream>

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StageTestInput::StageTestInput(std::string const& a_sId):
	AbstractStage(a_sId),
	m_pTestMesh(NULL)
{
	
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::Init()
{
	/// Input Test
	Engine::GetInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput01));
	Engine::GetInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput02));
	Engine::GetInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput03));

	Engine::GetInstance().GrabEventManager().RegisterCallbackMousePassiveMotion(
		EventManager::CallbackMouseMotion(this,&StageTestInput::TestInputMouse));

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
	Engine::GetInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput01));
	Engine::GetInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput02));
	Engine::GetInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput03));

	delete m_pTestMesh;
	m_pTestMesh = NULL;
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
bool StageTestInput::TestInputMouse(unsigned int a_uMouseX, unsigned int a_uMouseY)
{
	std::cout<<"Input method Mouse : "<<a_uMouseX<<" "<<a_uMouseY<<std::endl;
	return true;
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StageTestInput::_Render()
{
	// Render a mesh
	gluLookAt(0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
	m_pTestMesh->Render();
}