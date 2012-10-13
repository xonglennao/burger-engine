/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/08/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/


#ifndef __STAGEMAINS3DCENE_H__
#define __STAGEMAINS3DCENE_H__

#include "BurgerEngine/Core/AbstractStage.h"
#include "BurgerEngine/Input/EventManager.h"
class Timer;
class RenderingContext;

/// \class	MainStage
/// \brief	Used to display 3D scenes
class MainStage: public AbstractStage
{
public:

	/// \brief Constuctor, call parent
	MainStage(std::string const& a_sId);
	~MainStage();

	bool Init();

	/// \brief Simple overriden update method
	void Update();

	///\ \brief Manage keyboard input
	bool OnKeyUp(unsigned char a_cKey);
	bool OnKeyDown(unsigned char a_cKey);
	bool OnMouseMoved(unsigned int a_uX, unsigned int a_uY);
	bool OnJoystickMoved(unsigned int a_uStick, float a_fXValue, float a_fYValue);
	bool OnPadButtonPressed(EventManager::PAD_BUTTON iButton, bool bPressed);
};


#endif //__STAGEMAINS3DCENE_H__
