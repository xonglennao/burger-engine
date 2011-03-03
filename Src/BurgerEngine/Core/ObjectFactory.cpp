#include <assert.h>
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/RenderComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ObjectFactory::Init()
{
	m_sPath = "../Data/Objects/";
}

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
CompositeComponent* ObjectFactory::LoadObject(std::string const& a_rsFileName)
{
	std::string m_sFullPath;

	m_sFullPath.append(m_sPath);
	m_sFullPath.append(a_rsFileName);
	m_sFullPath.append(".bcp.xml");

	//Load the file
	TiXmlDocument * pDocument = new TiXmlDocument(m_sFullPath);

	if(!pDocument->LoadFile())
	{
		std::cerr << "[ReadXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
	}

	TiXmlElement * pRoot = pDocument->FirstChildElement( "object" );
	CompositeComponent* pComposite = NULL;
	if( pRoot )
	{
		///----------------------------------------------------------
		std::string sObjectName;
		pRoot->QueryValueAttribute<std::string>("name",&sObjectName);

		TiXmlElement * pComponent = pRoot->FirstChildElement( "components" );

		//We know for sure it will be a composite
		//Well.. we should check this somewhere
		CompositeComponent* pComposite =static_cast<CompositeComponent*>(LoadComponent(*pComponent));
		

	}

	//Todo stor into the list
	return pComposite;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractComponent* ObjectFactory::LoadComponent(TiXmlElement const& a_rComponentXml)
{
	AbstractComponent* pComponent = NULL;

	//Retrieve ID
	std::string sId;
	a_rComponentXml.QueryValueAttribute<std::string>("id",&sId);

	//Transform to switch case with an int?
	if (sId == "render")
	{
		//Create the component
		pComponent = new RenderComponent();

	}
	else if(sId == "composite")
	{
		pComponent = new CompositeComponent();
	}

	//Get Parameters
	//std::map<std::string, void*> mParameters;
	//If would be nice to have a map of parameter, however, the object factory
	//does not know how to create the parameter (maybe we have a map or something like that)
	// It would create a map of <string, void>
	// I want to do this in order to avoid moving around the TIXmlElement
	//Init with the parameter node
	TiXmlElement const* pParameter = a_rComponentXml.FirstChildElement("parameters");
	pComponent->Initialize(*pParameter);

	return pComponent;

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent* ObjectFactory::CreateComponentInstance(std::string const& a_rsID)
{
	//We look for the instance to create
	std::map<std::string , CompositeComponent*>::iterator iter = m_mObjects.find(a_rsID);
	
	//The instance is not created, we load it so far never store
	if (iter != m_mObjects.end())
	{
		CompositeComponent* pObject = new CompositeComponent(*iter->second);
		return pObject;
	}
	else
	{
		assert(false);
	}
	return NULL;

}


