#include "EventManager.h"
#include "SFMLInputManager.h"
#include <iostream>


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::Init(bool bIsWiimoteEnabled)
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

	if (bIsWiimoteEnabled)
	{
		ADD_LOG_MESSAGE("connecting Wiimote...");
		m_pWiimote = new wiimote();
		m_pWiimote->ChangedCallback	= &EventManager::CallbackWiiState;
		m_pWiimote->CallbackTriggerFlags = (state_change_flags)(CHANGED_ALL );
		m_iTimerTest = 0;
		while(!m_pWiimote->Connect(wiimote::FIRST_AVAILABLE)) {
			Sleep(1000);
		}
		m_pWiimote->SetLEDs(1 | 1<<3);

	}
	

	for(unsigned i = 0; i < PAD_BUTTON_MAX; ++i)
	{
		m_pButtonPressed[i] = false;
		m_aButtonPressedWiimote[i] = false;
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

	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_1 ] = wiimote_state::buttons::_A;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_2 ] = wiimote_state::buttons::_B ;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_3 ] = wiimote_state::buttons::ONE ;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_4 ] = wiimote_state::buttons::TWO ;

	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_L ] = 0;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_R ] = 0;

	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_L_SITCK ] = 0;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_R_STICK ] = 0;

	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_UP ] = wiimote_state::buttons::UP;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_DOWN ] = wiimote_state::buttons::DOWN;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_LEFT ] = wiimote_state::buttons::LEFT;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_RIGHT ] = wiimote_state::buttons::RIGHT;

	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_START ] = wiimote_state::buttons::PLUS;
	m_oBaseButtonToWiimoteButton[ PAD_BUTTON_SELECT ] = wiimote_state::buttons::MINUS;

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

	if(m_pWiimote)
	{
		m_pWiimote->Disconnect();
		delete m_pWiimote;
		m_pWiimote = NULL;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::ProcessEventList()
{
	SFMLInputManager::ProcessEvents();

	if (m_pWiimote)
	{
		ProcessWiimoteEvents();
	}
	else if (m_pXboxController)
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
void EventManager::ProcessWiimoteEvents()
{
	if (m_pWiimote)
	{
		m_pWiimote->RefreshState();
		//m_pWiimote->SetLEDs(1 | 1<<3);
	
		switch(m_pWiimote->ExtensionType)
		{
			case wiimote_state::NONE:
		
			break;
			
			case wiimote_state::PARTIALLY_INSERTED:
			{
				ADD_LOG_MESSAGE("Wiimote partially inserted");
			}
			break;
			
			// -- Nunchuk --
			case wiimote_state::NUNCHUK:
			{
				DispatchJoystickValue(0, m_pWiimote->Nunchuk.Joystick.X,  m_pWiimote->Nunchuk.Joystick.Y );

				/*BRIGHT_WHITE; _tprintf(_T("Nunchuk   "));

				// Buttons:
				CYAN		; _tprintf(_T("Buttons: ")); WHITE; _tprintf(_T("["));
				BRIGHT_WHITE; _tprintf(remote.Nunchuk.C? _T("C") : _T(" "));
				CYAN		; _tprintf(_T("|"));
				BRIGHT_WHITE; _tprintf(remote.Nunchuk.Z? _T("Z") : _T(" "));
				WHITE		; _tprintf(_T("]   "));
				// Joystick:
				CYAN		; _tprintf(_T("Joystick:  "));
				WHITE		; _tprintf(_T("X %+2.3f  Y %+2.3f\n"),
									   remote.Nunchuk.Joystick.X,
									   remote.Nunchuk.Joystick.Y);
				// Acceleration:
				CYAN		; _tprintf(_T("    Accel:"));
				WHITE		; _tprintf(_T("  X %+2.3f  Y %+2.3f  Z %+2.3f  \n"),
									   remote.Nunchuk.Acceleration.X,
									   remote.Nunchuk.Acceleration.Y,
									   remote.Nunchuk.Acceleration.Z);
				
				// Orientation estimate (shown red if last valid update is aging):
				CYAN		; _tprintf(_T("   Orient:"));
				WHITE		; _tprintf(_T("  UpdateAge %3u  "),
								remote.Nunchuk.Acceleration.Orientation.UpdateAge);
				//  show if the last orientation update is aging
				if(remote.Nunchuk.Acceleration.Orientation.UpdateAge > 10)
					RED;
				_tprintf(_T("Pitch %4ddeg  Roll %4ddeg  \n")
					     _T("                           (X %+.2f  Y %+.2f  Z %+.2f)      \n"),
						 (int)remote.Nunchuk.Acceleration.Orientation.Pitch,
						 (int)remote.Nunchuk.Acceleration.Orientation.Roll ,
						 remote.Nunchuk.Acceleration.Orientation.X,
						 remote.Nunchuk.Acceleration.Orientation.Y,
  						 remote.Nunchuk.Acceleration.Orientation.Z);*/
				break;
			}
				
		}

		for(unsigned int i = 0; i < PAD_BUTTON_MAX; ++i)
		{
			unsigned short uMask = m_oBaseButtonToWiimoteButton[ static_cast<PAD_BUTTON>(i) ];
			ProcessButtonPressed( static_cast<PAD_BUTTON>(i), ((m_pWiimote->Button.Bits & uMask) != 0) );
		}

		//Old version to discard
		/*for(unsigned int bit=0; bit<16; ++bit)
		{
			unsigned int mask = 1 << bit;
			// skip unused bits
			if((wiimote_state::buttons::ALL & mask) == 0)
				continue;

			const TCHAR* button_name = wiimote::ButtonNameFromBit[bit];
			bool		 pressed	 = ((m_pWiimote->Button.Bits & mask) != 0);
			if (pressed)
			{
				ADD_LOG_MESSAGE(button_name);

			}
			
		}*/
		
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

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EventManager::CallbackWiiState(wiimote	&remote, state_change_flags  changed, wiimote_state const& new_state)
{
	if(changed & CONNECTED)
	{
		ADD_LOG_MESSAGE("Wiimote is now connected");
		if(new_state.bExtension)
			remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT); // no IR dots
		else
			remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);		//    IR dots
		

	}
	else if (changed & CONNECTION_LOST)
	{
		ADD_LOG_MESSAGE("Wiimote is now disconnected");
	}
	else if(changed & EXTENSION_CONNECTED)
	{
		remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT);
	}
	// extension was just disconnected:
	else if(changed & EXTENSION_DISCONNECTED)
	{
		// use a non-extension report mode (this gives us back the IR dot sizes)
		remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
	}

}


