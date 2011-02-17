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

//forward declaration
class CompositeComponent;

/// \brief
/// \class ObjectFactory
class ObjectFactory
{
public:
	/// \brief Init
	void Init();

	/// \brief Clear all the map
	void Terminate();

	/// \brief Load a Object file and keep it into it's map
	bool LoadObject(std::string const& a_rsFileName);

	/// \brief Try to create an object using the ID
	/// \param a_rsID the unique ID... should become an int as it would become much faster
	CompositeComponent* CreateObject(std::string const& a_rsID);

private:
	/// The map which contain all the object to spawn
	std::map<std::string, CompositeComponent*> m_mObjects;
};

#endif //_OBJECTFACTORY_H__