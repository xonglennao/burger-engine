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
class Timer;
class RenderingContext;
/// \class	StageMain3DScene
/// \brief	Used to display 3D scenes
class StageMain3DScene: public AbstractStage
{
public:

	/// \brief Constuctor, call parent
	StageMain3DScene(std::string const& a_sId);
	~StageMain3DScene();

	bool Init();

	/// \brief Simple overriden update method
	void Update();

	///\ \brief Manage keyboard input
	bool OnKeyUp(unsigned char a_cKey);
	bool OnKeyDown(unsigned char a_cKey);
	bool OnMouseMoved(unsigned int a_uX, unsigned int a_uY);
	bool OnJoystickMoved(unsigned int a_uStick, float a_fXValue, float a_fYValue);
};


#endif //__STAGEMAINS3DCENE_H__
