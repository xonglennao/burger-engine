#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/RenderComponent.h"
#include "BurgerEngine/Core/LightComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/External/TinyXml/TinyXml.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::CompositeComponent(CompositeComponent* a_pParent):
	AbstractComponent(COMPOSITE, a_pParent)
{
	AbstractComponent::SetPos(vec3(0,0,0));
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
void CompositeComponent::SetPos( vec3 const& a_vValue )
{
	//Set the component position
	AbstractComponent::SetPos(a_vValue);

	//For all render component we let them know the new pos
	//Al this shoudl be kinda different
	//We change the pos, and when the Mesh needs to be rendered, it take the 
	//parent matrice and adds it's own
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		if (pComponent->GetType() == RENDER)
		{
			RenderComponent* pRenderComponent = static_cast<RenderComponent*>(pComponent);
			pRenderComponent->UpdatePos();
		}
		else if (pComponent->GetType() == LIGHT)
		{
			LightComponent* pLightComponent = static_cast<LightComponent*>(pComponent);
			pLightComponent->UpdatePos();
		}
	}

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::SetScale( float const a_fValue )
{
	/// \todo maybe the composite component should hold a scale value
	//For all component that have a scale issue
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		if (pComponent->GetType() == RENDER)
		{
			RenderComponent* pRenderComponent = static_cast<RenderComponent*>(pComponent);
			pRenderComponent->UpdateScale(a_fValue);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::SetRotation( vec3 const& a_vValue )
{
	/// \todo maybe the composite component should hold a scale value
	//For all component that have a scale issue
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		if (pComponent->GetType() == LIGHT)
		{
			LightComponent* pLightComponent = static_cast<LightComponent*>(pComponent);
			pLightComponent->UpdateRotation(a_vValue);
		}
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

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
std::vector<AbstractComponent*> CompositeComponent::TryGrabComponentVectorByType( ComponentType a_eType )
{
	std::vector<AbstractComponent*> vToReturn;
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		if (pComponent->GetType() == a_eType)
		{
			vToReturn.push_back(pComponent);
		}
	}
	return vToReturn;
}