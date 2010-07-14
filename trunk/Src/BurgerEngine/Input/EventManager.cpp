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
	m_vKeyboardUpKeyCallbacks.clear();
	m_vKeyboardDownKeyCallbacks.clear();
	m_vMousePassiveMotionCallbacks.clear();
	m_vMouseActiveMotionCallbacks.clear();
	m_vResizeCallbacks.clear();
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
void EventManager::RegisterCallbackKeyboardUpKey(CallbackKeyboard& a_rCallback)
{
	m_vKeyboardUpKeyCallbacks.push_back(a_rCallback);
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackKeyboardUpKey(CallbackKeyboard& a_rCallback)
{
	std::vector<CallbackKeyboard>::iterator it = std::find(m_vKeyboardUpKeyCallbacks.begin(),
		m_vKeyboardUpKeyCallbacks.end(),
		a_rCallback);
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
void EventManager::RegisterCallbackKeyboardDownKey(CallbackKeyboard& a_rCallback)
{
	m_vKeyboardDownKeyCallbacks.push_back(a_rCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackKeyboardDownKey(CallbackKeyboard& a_rCallback)
{
	std::vector<CallbackKeyboard>::iterator it = std::find(m_vKeyboardDownKeyCallbacks.begin(),
		m_vKeyboardDownKeyCallbacks.end(),
		a_rCallback);
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
void EventManager::RegisterCallbackMousePassiveMotion(CallbackMouseMotion& a_rCallback)
{
	m_vMousePassiveMotionCallbacks.push_back(a_rCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackMousePassiveMotion(CallbackMouseMotion& a_rCallback)
{
	std::vector<CallbackResize>::iterator it = std::find(m_vMousePassiveMotionCallbacks.begin(),
		m_vMousePassiveMotionCallbacks.end(),
		a_rCallback);
	if (it != m_vMousePassiveMotionCallbacks.end())
	{
		m_vMousePassiveMotionCallbacks.erase(it);
	}
	else
	{
		std::cerr<<"WARNING: CallBack not found."<<std::endl;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::RegisterCallbackMouseActiveMotion(CallbackMouseMotion& a_rCallback)
{
	m_vMouseActiveMotionCallbacks.push_back(a_rCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackMouseActiveMotion(CallbackMouseMotion& a_rCallback)
{
	std::vector<CallbackResize>::iterator it = std::find(m_vMouseActiveMotionCallbacks.begin(),
		m_vMouseActiveMotionCallbacks.end(),
		a_rCallback);
	if (it != m_vMouseActiveMotionCallbacks.end())
	{
		m_vMouseActiveMotionCallbacks.erase(it);
	}
	else
	{
		std::cerr<<"WARNING: CallBack not found."<<std::endl;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::RegisterCallbacResize(CallbackResize& a_rCallback)
{
	m_vResizeCallbacks.push_back(a_rCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbacResize(CallbackResize& a_rCallback)
{
	std::vector<CallbackResize>::iterator it = std::find(m_vResizeCallbacks.begin(),
		m_vResizeCallbacks.end(),
		a_rCallback);
	if (it != m_vResizeCallbacks.end())
	{
		m_vResizeCallbacks.erase(it);
	}
	else
	{
		std::cerr<<"WARNING: CallBack not found."<<std::endl;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchKeyboardUpKeyEvent(unsigned char a_cKey) const
{
	std::vector<CallbackKeyboard>::const_iterator it = m_vKeyboardUpKeyCallbacks.begin() ;
	//The call back can specify that onceits routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vKeyboardUpKeyCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_cKey);
		++it;
	}

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchKeyboardDownKeyEvent(unsigned char a_cKey) const
{
	std::vector<CallbackKeyboard>::const_iterator it = m_vKeyboardDownKeyCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vKeyboardDownKeyCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_cKey);
		++it;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchMouseDownClick(int a_iButton, int a_iState, int a_iXCoordinates, int a_iYCoordinates) const
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchMousePassiveMotion(unsigned int a_iXCoordinates,unsigned int a_iYCoordinates) const
{
	std::vector<CallbackMouseMotion>::const_iterator it = m_vMousePassiveMotionCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vMousePassiveMotionCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_iXCoordinates, a_iYCoordinates);
		++it;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchMouseActiveMotion(unsigned int a_iXCoordinates, unsigned int a_iYCoordinates) const
{
	std::vector<CallbackMouseMotion>::const_iterator it = m_vMouseActiveMotionCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vMouseActiveMotionCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_iXCoordinates, a_iYCoordinates);
		++it;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchResize(unsigned int a_uHeight, unsigned int a_uWidth) const
{
	std::vector<CallbackResize>::const_iterator it = m_vResizeCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vResizeCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_uHeight, a_uWidth);
		++it;
	}
}