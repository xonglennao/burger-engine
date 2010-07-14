/*************************************
*
*		BurgerEngine Project
*		
*		Created :	11/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/


#ifndef __STAGETESTINPUT_H__
#define __STAGETESTINPUT_H__

#include "BurgerEngine/Core/AbstractStage.h"

/// \class	StageTestInput
/// \brief	Use for Testing Input
///	\TODO	Create unit test project, better than a shit load of test scattered into the project

class StageTestInput: public AbstractStage
{
public:

	/// \brief Constuctor, call parent
	StageTestInput(std::string const& a_sId):AbstractStage(a_sId){}
	~StageTestInput();

	bool Init();

	/// \brief Callback Input test
	bool TestInput01(unsigned char a_cKey);
	bool TestInput02(unsigned char a_cKey);
	bool TestInput03(unsigned char a_cKey);
	bool TestInputMouse(unsigned int a_uMouseX, unsigned int a_uMouseY);

private:

	/// \brief No display OVERRIDE
	void _Render(){}


};


#endif //__STAGETESTINPUT_H__
