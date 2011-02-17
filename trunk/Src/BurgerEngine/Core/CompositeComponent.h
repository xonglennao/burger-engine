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
#ifndef __COMPOSITECOMPONENT_H__
#define __COMPOSITECOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"

class CompositeComponent : public AbstractComponent
{
public:
	/// \brief constructor
	CompositeComponent();

	/// \brief Copy constructor //clone???
	CompositeComponent(AbstractComponent const& a_rToCopy);

	/// \brief Destructor
	~CompositeComponent();


protected:
	/// \brief Inner update
	void _Update();

};

#endif //__COMPOSITECOMPONENT_H_