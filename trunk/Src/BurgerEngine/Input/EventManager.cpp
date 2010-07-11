#include "EventManager.h"
#include "SFMLInputManager.h"

#include <iostream>


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::Init()
{
	/// We are just using SFML, but we should do a AbstractInputManager
	SFMLInputManager::InitializeInput();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::Clear()
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::ProcessEventList()
{
	SFMLInputManager::ProcessEvents();


}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::RegisterCallbackKeyboardUpKey(CallbackKeyboard a_oCallback)
{
	m_vKeyboardUpKeyCallbacks.push_back(a_oCallback);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackKeyboardUpKey(CallbackKeyboard a_oCallback)
{
		std::vector<CallbackKeyboard>::iterator it = std::find(m_vKeyboardUpKeyCallbacks.begin(),
		m_vKeyboardUpKeyCallbacks.end(),
		a_oCallback);
	if (it != m_vKeyboardUpKeyCallbacks.end())
	{
		m_vKeyboardUpKeyCallbacks.erase(it);
	}
	else
	{
		std::cerr<<"WARNING: CallBack not found."<<std::endl;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::RegisterCallbackKeyboardDownKey(CallbackKeyboard a_oCallback)
{
	m_vKeyboardDownKeyCallbacks.push_back(a_oCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackKeyboardDownKey(CallbackKeyboard a_oCallback)
{
	std::vector<CallbackKeyboard>::iterator it = std::find(m_vKeyboardDownKeyCallbacks.begin(),
		m_vKeyboardDownKeyCallbacks.end(),
		a_oCallback);
	if (it != m_vKeyboardDownKeyCallbacks.end())
	{
		m_vKeyboardDownKeyCallbacks.erase(it);
	}
	else
	{
		std::cerr<<"WARNING: CallBack not found."<<std::endl;
	}

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::RegisterCallbackMouseDownClick(void* a_pObject,
	void (*a_pFunction)(int a_iButton, int a_iState, int a_iXCoordinates, int a_iYCoordinates))
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackMouseDownClick()
{

}
	
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::RegisterCallbackMousePassiveMotion(void* a_pObject, void (*a_pFunction)(int a_iXCoordinates, int a_iYCoordinates))
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackMousePassiveMotion()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::RegisterCallbackMouseActiveMotion(void* a_pObject, void (*a_pFunction)(int a_iXCoordinates, int a_iYCoordinates))
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackMouseActiveMotion()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchKeyboardUpKeyEvent(unsigned char a_cKey)
{
	std::vector<CallbackKeyboard>::iterator it = m_vKeyboardUpKeyCallbacks.begin() ;
	//The call back can specify that onceits routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vKeyboardUpKeyCallbacks.end() || bContinue)
	{
		bContinue = (*it)(a_cKey);
	}

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchKeyboardDownKeyEvent(unsigned char a_cKey)
{
	std::vector<CallbackKeyboard>::iterator it = m_vKeyboardDownKeyCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vKeyboardDownKeyCallbacks.end() || bContinue)
	{
		bContinue = (*it)(a_cKey);
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchMouseDownClick(int a_iButton, int a_iState, int a_iXCoordinates, int a_iYCoordinates)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchMousePassiveMotion(int a_iXCoordinates, int a_iYCoordinates)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchMouseActiveMotion(int a_iXCoordinates, int a_iYCoordinates)
{
}