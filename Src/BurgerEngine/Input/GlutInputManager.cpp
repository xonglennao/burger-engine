#include "GlutInputManager.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "BurgerEngine/Core/Engine.h"
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
	Engine::GetInstance().GetEventManager().DispatchKeyboardUpKeyEvent(a_cKey);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnKeyboardDown(unsigned char a_cKey,int a_iX, int a_iY)
{
	switch(a_cKey)
	{ 
	case 27:
		    //Shoudl send a quit signal it not up to this class to tell "Engine you have to die!"
			Engine::GetInstance().SetTerminate(true);
			exit(0);
		break;
	default:
			Engine::GetInstance().GetEventManager().DispatchKeyboardDownKeyEvent(a_cKey);
		break;
	}

}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnMouseClick(int a_iButton, int a_iState, int a_iX, int a_iY)
{
	Engine::GetInstance().GetEventManager().DispatchMouseDownClick(a_iButton, a_iState, a_iX, a_iY);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnMouseMotion(int a_iX, int a_iY)
{
	Engine::GetInstance().GetEventManager().DispatchMouseActiveMotion(a_iX, a_iY);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GlutInputManager::OnMousePassiveMotion(int a_iX, int a_iY)
{
	Engine::GetInstance().GetEventManager().DispatchMousePassiveMotion(a_iX, a_iY);

}