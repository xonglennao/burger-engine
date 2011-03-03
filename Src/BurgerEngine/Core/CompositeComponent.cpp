#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/External/TinyXml/TinyXml.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::CompositeComponent()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::CompositeComponent(AbstractComponent const& a_rToCopy)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::~CompositeComponent()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::Initialize(TiXmlElement const& a_rParameters)
{
	//The composite will take it's value (position) and then load all other component
	TiXmlElement const* pPosition = a_rParameters.FirstChildElement("position");

	float fX, fY, fZ, fRx, fRy, fRz, fScale;

	//gets position & bounded volume information 
	pPosition->QueryFloatAttribute("x",&fX);
	pPosition->QueryFloatAttribute("y",&fY);
	pPosition->QueryFloatAttribute("z",&fZ);

	//Those might be optionnal, maybe just for the render?
	pPosition->QueryFloatAttribute("rX",&fRx);
	pPosition->QueryFloatAttribute("rY",&fRy);
	pPosition->QueryFloatAttribute("rZ",&fRz);
	pPosition->QueryFloatAttribute("scale",&fScale);

	//As long as we find components we add it to the composite components
	TiXmlElement const* pComponent = a_rParameters.FirstChildElement( "components" );
	while (pComponent)
	{
		AbstractComponent* pComponentObject = ObjectFactory::LoadComponent(*pComponent);

		m_vComponents.push_back(pComponentObject);

		//Get the next components
		pComponent = pComponent->NextSiblingElement( "components" );
	}

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::_Update()
{
	//FOR EACH MACRO
	for (std::vector<AbstractComponent*>::iterator itComponent = m_vComponents.begin(); itComponent != m_vComponents.end(); ++itComponent)
	{
		//Should check if need to be updated??? (think about render and phys)
		(*itComponent)->Update();
	}

}