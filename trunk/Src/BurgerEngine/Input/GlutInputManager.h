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

#ifndef __GLUTLINPUTMANAGER_H__
#define __GLUTLINPUTMANAGER_H__

///	\name	GlutInputManager.h
///	\brief	Lowlevel glut input callback routines
///			Will send event to the EventManager
///			Library specific;

class GlutInputManager
{
public:

	/// \brief	Set the GlutCallback
	static void InitializeInput();

	/// \brief	Glut callback 
	static void OnKeyboardUp(unsigned char a_cKey,int a_iX, int a_iY);
	static void OnKeyboardDown(unsigned char a_cKey,int a_iX, int a_iY);
	static void OnMouseClick(int a_iButton, int a_iState, int a_iX, int a_iY);
	static void OnMouseMotion(int a_iX, int a_iY);
	static void OnMousePassiveMotion(int a_iX, int a_iY);

private:


};

#endif //__GLUTLINPUTMANAGER_H__
