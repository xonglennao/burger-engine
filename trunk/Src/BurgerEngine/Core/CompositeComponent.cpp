#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/External/TinyXml/TinyXml.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::CompositeComponent(CompositeComponent* a_pParent):
	AbstractComponent(COMPOSITE, a_pParent)
{
	SetPos(vec3(0,0,0));
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::CompositeComponent(AbstractComponent const& a_rToCopy):
	AbstractComponent(COMPOSITE)
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
	float fX, fY, fZ;
	TiXmlElement const* pPositionXml = a_rParameters.FirstChildElement( "position" );
	if(pPositionXml)
	{
		pPositionXml->QueryFloatAttribute("x",&fX);
		pPositionXml->QueryFloatAttribute("y",&fY);
		pPositionXml->QueryFloatAttribute("z",&fZ);
		SetPos(vec3(fX,fY,fZ));
	}

	//As long as we find components we add it to the composite components
	TiXmlElement const* pComponentXml = a_rParameters.FirstChildElement( "component" );
	while (pComponentXml)
	{
		AbstractComponent* pComponentObject = ObjectFactory::CreateAndInitComponent(*pComponentXml, this);

		m_vComponents.push_back(pComponentObject);

		//Get the next components
		pComponentXml = pComponentXml->NextSiblingElement( "component" );
	}

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::_Update()
{
	//FOR EACH MACRO
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	//for (std::vector<AbstractComponent*>::iterator itComponent = m_vComponents.begin(); itComponent != m_vComponents.end(); ++itComponent)
	{
		//Should check if need to be updated??? (think about render and phys)
		(*itComponent)->Update();
	}

}



//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
/*AbstractComponent const& CompositeComponent::GetComponentByType( ComponentType a_eType ) const
{
	FOR_EACH_IT_CONST(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent const* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		if (pComponent->GetType() == a_eType)
		{
			/// \todo if we could know the type (with a template) we could
			/// simply do static_cast<t_type> *pComponent
			return *pComponent;
		}
	}

	// we should never arrive here, or we should have used the TrypGrabMethod
	assert(false);
	//Need to return an invalid ref
}*/


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
/*AbstractComponent& CompositeComponent::GrabComponentByType( ComponentType a_eType )
{
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		if (pComponent->GetType() == a_eType)
		{
			return *pComponent;
		}
	}
	
	// we should never arrive here, or we should have used the TrypGrabMethod
	assert(false);
	// need to return an invalid ref
}*/

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractComponent const* CompositeComponent::TryGetComponentByType( ComponentType a_eType ) const
{
	FOR_EACH_IT_CONST(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent const* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		if (pComponent->GetType() == a_eType)
		{
			return pComponent;
		}
	}
	return NULL;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractComponent* CompositeComponent::TryGrabComponentByType( ComponentType a_eType )
{
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		if (pComponent->GetType() == a_eType)
		{
			return pComponent;
		}
	}
	return NULL;
}
