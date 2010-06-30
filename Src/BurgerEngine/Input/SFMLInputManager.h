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
///	\brief	Lowlevel glut input callback routines
///			Will send event to the EventManager
///			Library specific;

class SFMLInputManager
{
public:

	/// \brief	Set the SFMLCallback
	static void InitializeInput();

	/// \brief Process All events and send it to callback
	static void ProcessEvents();

	/// \brief	SFML callback 
	static void OnKeyboardUp(unsigned char a_cKey,int a_iX, int a_iY);
	static void OnKeyboardDown(unsigned char a_cKey,int a_iX, int a_iY);
	static void OnMouseClick(int a_iButton, int a_iState, int a_iX, int a_iY);
	static void OnMouseMotion(int a_iX, int a_iY);
	static void OnMousePassiveMotion(int a_iX, int a_iY);

private:


};

#endif //__SFMLINPUTMANAGER_H__
