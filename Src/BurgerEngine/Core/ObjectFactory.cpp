#include "ObjetFactory.h"
#include <assert.h>

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ObjectFactory::Terminate()
{
	for (std::map<std::string , CompositeComponent*>::iterator iter = m_mObjects.begin() ; iter != m_mObjects.end() ; ++iter)
	{
		delete (*iter).second;
	}
	m_mObjects.clear();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool ObjectFactory::LoadObject(std::string const& a_rsFileName)
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent* ObjectFactory::CreateObject(std::string const& a_rsID)
{
	std::map<std::string , CompositeComponent*>::iterator iter;
	iter = m_mObjects.find(a_rsID);

	if (iter != m_mObjects.end())
	{
		CompositeComponent* pObject = new CompositeComponent(*iter->second);
		return pObject;
	}
	else
	{
		assert(false);
	}
}
