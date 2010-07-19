/*************************************
*
*		BurgerEngine Project
*		
*		Created :	28/06/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __SFMLLINPUTMANAGER_H__
#define __SFMLLINPUTMANAGER_H__

///	\name	SMFLInputManager.h
///	\brief	Lowlevel smlf input callback routines
///			Will send event to the EventManager
///			Library specific;

class SFMLInputManager
{
public:

	/// \brief	Set the SFMLCallback
	static void InitializeInput();

	/// \brief Process All events and send it to callback
	static void ProcessEvents();

private:


};

#endif //__SFMLINPUTMANAGER_H__
