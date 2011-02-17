/*************************************
*
*		BurgerEngine Project
*		
*		Created :	16/02/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __RENDERCOMPONENT_H__
#define __RENDERCOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"

class RenderComponent : public AbstractComponent
{
public:
	/// \brief constructor
	RenderComponent();

	/// \brief Copy constructor //clone???
	RenderComponent(AbstractComponent const& a_rToCopy);

	/// \brief Destructor
	~RenderComponent();


protected:
	/// \brief Inner update
	void _Update();

};

#endif //__RENDERCOMPONENT_H__