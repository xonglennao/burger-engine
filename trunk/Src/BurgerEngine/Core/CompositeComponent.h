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

#include <vector>

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

	/// \brief Load the component using a XML Node
	/// override
	void Initialize(TiXmlElement const& a_rParameters);

protected:
	/// \brief Inner update
	void _Update();

private:
	/// \brief Collection Of AbstractComponenet
	std::vector<AbstractComponent*> m_vComponents;
};

#endif //__COMPOSITECOMPONENT_H_