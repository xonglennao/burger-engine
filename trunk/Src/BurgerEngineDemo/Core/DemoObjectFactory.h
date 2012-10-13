/*************************************
*
*		BurgerEngine Project
*		
*		Created :	29/09/12
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/


#ifndef __DEMOOBJECTFACTORY_H__
#define __DEMOOBJECTFACTORY_H__

#include "BurgerEngine/Core/ObjectFactory.h"

/// \brief
/// \class DemoObjectFactory
class DemoObjectFactory : public ObjectFactory
{
public:

	void Terminate();

	/// \brief Static loading method just load a single component
	AbstractComponent* CreateComponentFromID( std::string& a_sID, CompositeComponent* a_pParent );
};

#endif //_DEMOOBJECTFACTORY_H__