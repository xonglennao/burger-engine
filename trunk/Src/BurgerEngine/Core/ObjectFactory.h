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


#ifndef __OBJECTFACTORY_H__
#define __OBJECTFACTORY_H__

#include <map>
#include <string>
#include "BurgerEngine/External/Math/Vector.h"
#include "BurgerEngine/Base/CommonBase.h"

//forward declaration
class CompositeComponent;
class AbstractComponent;
class TiXmlElement;

/// \brief
/// \class ObjectFactory
class ObjectFactory
{
public:
	/// \brief Init
	void Init();

	/// \brief Clear all the map
	void Terminate();

	/// \brief Load a Object from a xml file
	static AbstractComponent* LoadObject( const TiXmlElement * const pXmlElement, CompositeComponent* a_pParent = NULL );

	/// \brief Static loading method just load a single component
	static AbstractComponent* CreateAndInitComponent(TiXmlElement const& a_rParameters, CompositeComponent* a_pParent);
	
	/// \brief Try to create an object using the ID
	/// \param a_rsID the unique ID... should become an int as it would become much faster
	CompositeComponent* CreateComponentInstance(std::string const& a_rsID);

private:
	/// The map which contain all the object to spawn
	std::map<std::string, CompositeComponent*> m_mObjects;

	/// The Path to the loading of object
	//std::string m_sPath;
};

#endif //_OBJECTFACTORY_H__