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
///			So far works for input

#include <vector>

//#include "BurgerEngine/External/Loki/include/loki/Functor.h"

class EventManager
{
public:

	//typedef Loki::Functor<void,LOKI_TYPELIST_1(char)> CallbackKeyboard;

public:
	/// \brief Register callback when a key is release
	void RegisterCallbackKeyboardUpKey(void* a_pObject, void (*a_pFunction)(unsigned char a_cKey));
	void UnRegisterCallbackKeyboardUpKey(/*Callback*/);

	/// \brief Register callbakc when key is pressed
	void RegisterCallbackKeyboardDownKey(void* a_pObject, void (*a_pFunction)(unsigned char a_cKey));
	void UnRegisterCallbackKeyboardDownKey(/**/);

	/// \brief Register callbakc when mouse is clicked
	void RegisterCallbackMouseDownClick(void* a_pObject,
		void (*a_pFunction)(int a_iButton, int a_iState, int a_iXCoordinates, int a_iYCoordinates));
	void UnRegisterCallbackMouseDownClick();
	
	/// \brief Register when mouse is moved without button pressed
	void RegisterCallbackMousePassiveMotion(void* a_pObject, void (*a_pFunction)(int a_iXCoordinates, int a_iYCoordinates));
	void UnRegisterCallbackMousePassiveMotion();

	/// \brief Register when mouse is moved with button pressed
	void RegisterCallbackMouseActiveMotion(void* a_pObject, void (*a_pFunction)(int a_iXCoordinates, int a_iYCoordinates));
	void UnRegisterCallbackMouseActiveMotion();

private:

	//Vector containing the callbacks
	//std::vector<callback> m_vKeyboardUpKeyCallbacks;
	//std::vector<callback> m_vKeyboardDownKeyCallbacks;
	//std::vector<callback> m_vMouseDownClickCallbacks;
	//std::vector<callback> m_vMousePassiveMotionCallbacks;
	//std::vector<callback> m_vMouseActiveMotionCallbacks;


};

#endif //__EVENTMANAGER_H__
