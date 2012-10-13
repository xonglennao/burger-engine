#include <assert.h>
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/RenderComponent.h"
#include "BurgerEngine/Core/LightComponent.h"
#include "BurgerEngine/Core/ParticleComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ObjectFactory::Init()
{
	//m_sPath = "../Data/Objects/";
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
AbstractComponent* ObjectFactory::LoadObject( const TiXmlElement * const pXmlElement, CompositeComponent* a_pParent )
{
	std::string sFullPath;
	std::string sFileName;
	const TiXmlElement * pFileNameXml= pXmlElement->FirstChildElement( "file" );
	if( pFileNameXml )
	{
		sFileName = pFileNameXml->GetText();
	}
	
	sFullPath.append( "../Data/Objects/" );
	sFullPath.append( sFileName );
	sFullPath.append(".bcp.xml");

	//Load the file
	TiXmlDocument * pDocument = new TiXmlDocument( sFullPath );

	if(!pDocument->LoadFile())
	{
		ADD_ERROR_MESSAGE( std::string("Loading : ") << pDocument->ErrorDesc() << std::string(" ") << sFileName );
		return NULL;
	}

	TiXmlElement * pComponentXML = pDocument->FirstChildElement( "component" );
	AbstractComponent* pComponent = CreateAndInitComponent( *pComponentXML, a_pParent );
	
	//a ressource component can override the component postion/rotation/scale
	if( pComponent )
	{
		float x, y, z, rX, rY, rZ, scale;

		TiXmlElement const* pPositionXml = pXmlElement->FirstChildElement( "position" );
		if( pPositionXml )
		{
			//gets position & bounded volume information 
			pPositionXml->QueryFloatAttribute("x",&x);
			pPositionXml->QueryFloatAttribute("y",&y);
			pPositionXml->QueryFloatAttribute("z",&z);

			pPositionXml->QueryFloatAttribute("rX",&rX);
			pPositionXml->QueryFloatAttribute("rY",&rY);
			pPositionXml->QueryFloatAttribute("rZ",&rZ);

			pPositionXml->QueryFloatAttribute("scale",&scale);
			pComponent->SetPos( vec3(x,y,z) );
			pComponent->SetRotation( vec3(rX,rY,rZ) );
			pComponent->SetScale( scale );
		}
	}

	return pComponent;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractComponent* ObjectFactory::CreateAndInitComponent(TiXmlElement const& a_rComponentXml,  CompositeComponent* a_pParent )
{
	AbstractComponent* pComponent = NULL;

	//Retrieve ID
	std::string sId;
	a_rComponentXml.QueryValueAttribute<std::string>("id",&sId);

	pComponent = CreateComponentFromID(sId, a_pParent);

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
AbstractComponent* ObjectFactory::CreateComponentFromID( std::string& a_sID, CompositeComponent* a_pParent )
{
	//Transform to switch case with an int?
	if (a_sID == "render")
	{
		//Create the component
		return new RenderComponent(a_pParent);
	}
	else if(a_sID == "composite")
	{
		return new CompositeComponent(a_pParent);
	}
	else if(a_sID == "light")
	{
		return new LightComponent(a_pParent);
	}
	else if(a_sID == "particle")
	{
		return new ParticleComponent(a_pParent);
	}
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


