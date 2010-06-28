#include "GlutInputManager.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

///TEMP	: To have the exit(0)
#include <iostream>
#include "BurgerEngine/Core/StageManager.h"
///Temp: We will access it by the engine.
#include "EventManager.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::InitializeInput()
{
	//Set the glut input callback
	glutKeyboardFunc(OnKeyboardDown);
	glutKeyboardUpFunc(OnKeyboardUp);

	//glutSpecialFunc(keyboardSpecialGL);
  
	glutMouseFunc(OnMouseClick);
	glutMotionFunc(OnMouseMotion);
	glutPassiveMotionFunc(OnMousePassiveMotion);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnKeyboardUp(unsigned char a_cKey,int a_iX, int a_iY)
{
	///Engine.GetEventManager().DispatchKeyUp();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnKeyboardDown(unsigned char a_cKey,int a_iX, int a_iY)
{
	/// \TODO	The quit routine will go into the Engine.
	/// 		It will safelty quit everything
	switch(a_cKey)
	{ 
	case 27:
		    //Engine->KILLll!
			StageManager::KillInstance();
			exit(0);
		break;
	default:
		break;
	}

}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnMouseClick(int a_iButton, int a_iState, int a_iX, int a_iY)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnMouseMotion(int a_iX, int a_iY)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnMousePassiveMotion(int a_iX, int a_iY)
{


}