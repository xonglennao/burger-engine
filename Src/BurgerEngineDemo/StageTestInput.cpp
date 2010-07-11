#include "StageTestInput.h"
#include "BurgerEngine/Input/EventManager.h"
#include "BurgerEngine/Core/Engine.h"
#include <iostream>

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageTestInput::Init()
{

	Engine::GetInstance().GetEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput01));
	Engine::GetInstance().GetEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput02));
	Engine::GetInstance().GetEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput03));

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StageTestInput::~StageTestInput()
{
	Engine::GetInstance().GetEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput01));
	Engine::GetInstance().GetEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput02));
	Engine::GetInstance().GetEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageTestInput::TestInput03));

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


