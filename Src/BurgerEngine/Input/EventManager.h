/*************************************
*
*		BurgerEngine Project
*		
*		Created :	27/06/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

///	\name EventManager.h
///	\brief	Register and dispatch eventCallbacks
///			So far works for input but could work with other stuff
///			Here an exemple how to access control over keyboard
///			You create a function bool MyClass::Foo(unsigned char) in you class
///			Must return Fasle is want other callback to be not be called. (Pop Up menu)
///			Then you call the following
///			Engine::GetInstance().GetEventManager().RegisterCallbackKeyboardUpKey(CallBackKeyBoard<this,MyClass::Foo>);
///			To unregister
///			Engine::GetInstance().GetEventManager().UnRegisterCallbackKeyboardUpKey(CallBackKeyBoard<this,MyClass::Foo>);

#include <vector>
//#include "BurgerEngine/Base/Functor.h"
#include "BurgerEngine/External/Loki/Include/loki/Functor.h"


class EventManager
{
public:

	//Call back typedef
	typedef Loki::Functor<bool,LOKI_TYPELIST_1(unsigned char)> CallbackKeyboard;
	typedef Loki::Functor<bool,LOKI_TYPELIST_2(unsigned int, unsigned int)> CallbackMouseMotion;
	typedef Loki::Functor<bool,LOKI_TYPELIST_3(
		unsigned int, //Button
		unsigned int, // Coordinates
		unsigned int)> CallbackMouseButton;
	typedef Loki::Functor<bool,LOKI_TYPELIST_2(unsigned int, unsigned int)> CallbackResize;

public:

	/// \brief	Initialize method (set what input manager to use....)
	void Init();

	/// \brief	Clear all callback list
	void Clear();

	/// \brief	Has to be called each frame
	///			Will clear the event queue
	void ProcessEventList();

	/// \brief Register callback when a key is release
	void RegisterCallbackKeyboardUpKey(CallbackKeyboard& a_rCallback);
	void UnRegisterCallbackKeyboardUpKey(CallbackKeyboard& a_rCallback);

	/// \brief Register callback when key is pressed
	void RegisterCallbackKeyboardDownKey(CallbackKeyboard& a_rCallback);
	void UnRegisterCallbackKeyboardDownKey(CallbackKeyboard& a_rCallback);

	/// \brief Register callbakc when mouse is clicked
	void RegisterCallbackMouseDownClick(CallbackMouseButton& a_rCallback);
	void UnRegisterCallbackMouseDownClick(CallbackMouseButton& a_rCallback);
	
	/// \brief Register when mouse is moved without button pressed
	void RegisterCallbackMousePassiveMotion(CallbackMouseMotion& a_rCallback);
	void UnRegisterCallbackMousePassiveMotion(CallbackMouseMotion& a_rCallback);

	/// \brief Register when mouse is moved with button pressed
	void RegisterCallbackMouseActiveMotion(CallbackMouseMotion& a_rCallback);
	void UnRegisterCallbackMouseActiveMotion(CallbackMouseMotion& a_rCallback);

	/// \brief Register when screen is resized
	void RegisterCallbacResize(CallbackResize& a_rCallback);
	void UnRegisterCallbacResize(CallbackResize& a_rCallback);

	///--------- Dispatch event ----------------
	/// \brief	Send the key event to every register Callback
	void DispatchKeyboardUpKeyEvent(unsigned char a_cKey) const;

	/// \brief	Send the key event to every register Callback
	void DispatchKeyboardDownKeyEvent(unsigned char a_cKey) const;

	/// \brief	Send the click event to every register Callback
	void DispatchMouseDownClick(int a_iButton, int a_iXCoordinates, int a_iYCoordinates) const;

	/// \brief	Send the release event to every register Callback
	void DispatchMouseUpClick(int a_iButton, int a_iXCoordinates, int a_iYCoordinates) const;

	/// \brief	Send the mouse motion event to every register Callback
	void DispatchMousePassiveMotion(unsigned int a_iXCoordinates, unsigned int a_iYCoordinates) const;

	/// \brief	Send the mouse motion to every register Callback
	void DispatchMouseActiveMotion(unsigned int a_iXCoordinates, unsigned int a_iYCoordinates) const;
	 
	/// \brief	Send the Resize event to every register callback
	void DispatchResize(unsigned int a_uHeight, unsigned int a_uWidth) const;


private:

	//Vector containing the callbacks
	std::vector<CallbackKeyboard> m_vKeyboardUpKeyCallbacks;
	std::vector<CallbackKeyboard> m_vKeyboardDownKeyCallbacks;
	std::vector<CallbackResize> m_vResizeCallbacks;
	std::vector<CallbackMouseMotion> m_vMousePassiveMotionCallbacks;
	std::vector<CallbackMouseMotion> m_vMouseActiveMotionCallbacks;
	std::vector<CallbackMouseButton> m_vMouseClickDownCallbacks;
	std::vector<CallbackMouseButton> m_vMouseClickUpCallbacks;
	
};

#endif //__EVENTMANAGER_H__
