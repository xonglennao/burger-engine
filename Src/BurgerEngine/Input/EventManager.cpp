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

	int iNumConnected;
	int* pPlayersConnected = XController::getConnectedPlayers(&iNumConnected);

	m_pXboxController = NULL;

	if(iNumConnected)
	{
		m_pXboxController = new XController( pPlayersConnected[0] );
	}


	for(unsigned i = 0; i < PAD_BUTTON_MAX; ++i)
	{
		m_pButtonPressed[i] = false;
	}

	m_oBaseButtonToXboxButton[ PAD_BUTTON_1 ] = XController::A ;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_2 ] = XController::B ;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_3 ] = XController::X ;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_4 ] = XController::Y ;

	m_oBaseButtonToXboxButton[ PAD_BUTTON_L ] = XController::LEFT_SHOULDER;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_R ] = XController::RIGHT_SHOULDER;

	m_oBaseButtonToXboxButton[ PAD_BUTTON_L_SITCK ] = XController::LEFT_THUMB;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_R_STICK ] = XController::RIGHT_THUMB;

	m_oBaseButtonToXboxButton[ PAD_BUTTON_UP ] = XController::DPAD_UP;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_DOWN ] = XController::DPAD_DOWN;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_LEFT ] = XController::DPAD_LEFT;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_RIGHT ] = XController::DPAD_RIGHT;

	m_oBaseButtonToXboxButton[ PAD_BUTTON_START ] = XController::START;
	m_oBaseButtonToXboxButton[ PAD_BUTTON_SELECT ] = XController::BACK;
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
	m_vMouseClickDownCallbacks.clear();
	m_vMouseClickUpCallbacks.clear();
	m_vResizeCallbacks.clear();

	if(m_pXboxController)
	{
		m_pXboxController->vibrate(0.0f, 0.0f);
		delete m_pXboxController;
		m_pXboxController = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::ProcessEventList()
{
	SFMLInputManager::ProcessEvents();
	
	if(m_pXboxController)
	{
		ProcessXboxControllerEvents();
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::ProcessXboxControllerEvents()
{
	m_pXboxController->update();

	if( m_pXboxController->isConnected() )
	{
		XController::StickState stick = m_pXboxController->getLeftStickState();
		DispatchJoystickValue(0,stick.dirX, stick.dirY );

		stick = m_pXboxController->getRightStickState();
		DispatchJoystickValue(1,stick.dirX, stick.dirY );

		for(unsigned int i = 0; i < PAD_BUTTON_MAX; ++i)
		{
			ProcessButtonPressed( static_cast<PAD_BUTTON>(i), m_pXboxController->isPressed(m_oBaseButtonToXboxButton[ static_cast<PAD_BUTTON>(i) ] ) );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::ProcessButtonPressed( PAD_BUTTON eButton, bool bPressed )
{
	if( bPressed )
	{
		if(!m_pButtonPressed[eButton])
		{
			m_pButtonPressed[eButton] = true;
			DispatchPadButtonPressed(eButton,true);
		}
	}
	else
	{
		if(m_pButtonPressed[eButton] == true)
		{
			m_pButtonPressed[eButton] = false;
			DispatchPadButtonPressed(eButton,false);
		}
	}
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
void EventManager::RegisterCallbackMouseDownClick(CallbackMouseButton& a_rCallback)
{
	m_vMouseClickDownCallbacks.push_back(a_rCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackMouseDownClick(CallbackMouseButton& a_rCallback)
{
	std::vector<CallbackMouseButton>::iterator it = std::find(m_vMouseClickDownCallbacks.begin(),
		m_vMouseClickDownCallbacks.end(),
		a_rCallback);
	if (it != m_vMouseClickDownCallbacks.end())
	{
		m_vMouseClickDownCallbacks.erase(it);
	}
	else
	{
		std::cerr<<"WARNING: CallBack not found."<<std::endl;
	}
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
void EventManager::RegisterCallbackResize(CallbackResize& a_rCallback)
{
	m_vResizeCallbacks.push_back(a_rCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackResize(CallbackResize& a_rCallback)
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
void EventManager::RegisterCallbackJoystick(CallbackXBoxJoystick& a_rCallback)
{
	m_vXBoxJoystickCallbacks.push_back(a_rCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackJoystick(CallbackXBoxJoystick& a_rCallback)
{
	std::vector<CallbackXBoxJoystick>::iterator it = std::find(m_vXBoxJoystickCallbacks.begin(),
		m_vXBoxJoystickCallbacks.end(),
		a_rCallback);
	if (it != m_vXBoxJoystickCallbacks.end())
	{
		m_vXBoxJoystickCallbacks.erase(it);
	}
	else
	{
		std::cerr<<"WARNING: CallBack not found."<<std::endl;
	}
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::RegisterCallbackPadButton(CallbackPadButtonPressed& a_rCallback)
{
	m_vPadButtonCallbacks.push_back(a_rCallback);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::UnRegisterCallbackPadButton(CallbackPadButtonPressed& a_rCallback)
{
	std::vector<CallbackPadButtonPressed>::iterator it = std::find(m_vPadButtonCallbacks.begin(),
		m_vPadButtonCallbacks.end(),
		a_rCallback);
	if (it != m_vPadButtonCallbacks.end())
	{
		m_vPadButtonCallbacks.erase(it);
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
void EventManager::DispatchMouseDownClick(int a_iButton, int a_iXCoordinates, int a_iYCoordinates) const
{
	std::vector<CallbackMouseButton>::const_iterator it = m_vMouseClickDownCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vMouseClickDownCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_iButton, a_iXCoordinates, a_iYCoordinates);
		++it;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchMouseUpClick( int a_iButton, int a_iXCoordinates, int a_iYCoordinates ) const
{
	std::vector<CallbackMouseButton>::const_iterator it = m_vMouseClickUpCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vMouseClickUpCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_iButton, a_iXCoordinates, a_iYCoordinates);
		++it;
	}
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
void EventManager::DispatchResize(unsigned int a_uWidth, unsigned int a_uHeight) const
{
	std::vector<CallbackResize>::const_iterator it = m_vResizeCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vResizeCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_uWidth, a_uHeight);
		++it;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchJoystickValue(unsigned int a_iStick, float a_iXValue, float a_iYValue) const
{
	std::vector<CallbackXBoxJoystick>::const_iterator it = m_vXBoxJoystickCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vXBoxJoystickCallbacks.end() && bContinue)
	{
		bContinue = (*it)(a_iStick, a_iXValue, a_iYValue);
		++it;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::DispatchPadButtonPressed(PAD_BUTTON iButton, bool bPressed) const
{
	std::vector<CallbackPadButtonPressed>::const_iterator it = m_vPadButtonCallbacks.begin() ;
	//The call back can specify that once its routine is done, all the other cannot exectute their routine.
	bool bContinue = true;
	while(it != m_vPadButtonCallbacks.end() && bContinue)
	{
		bContinue = (*it)(iButton, bPressed);
		++it;
	}
}


