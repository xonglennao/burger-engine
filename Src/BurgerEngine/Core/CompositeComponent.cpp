#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/RenderComponent.h"
#include "BurgerEngine/Core/LightComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/External/TinyXml/TinyXml.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::CompositeComponent(CompositeComponent* a_pParent)
	: AbstractComponent(COMPOSITE, a_pParent)
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
	//this is currently useless, there's only one composite component per sceneobject
	//the composite component uses the scene object position
	
	float x, y, z, rX, rY, rZ, scale;
	TiXmlElement const* pPositionXml = a_rParameters.FirstChildElement( "position" );
	if(pPositionXml)
	{
		pPositionXml->QueryFloatAttribute("x",&x);
		pPositionXml->QueryFloatAttribute("y",&y);
		pPositionXml->QueryFloatAttribute("z",&z);
		SetPos(vec3(x,y,z));

		pPositionXml->QueryFloatAttribute("rX",&rX);
		pPositionXml->QueryFloatAttribute("rY",&rY);
		pPositionXml->QueryFloatAttribute("rZ",&rZ);
		SetRotation(vec3(rX,rY,rZ));

		pPositionXml->QueryFloatAttribute("scale",&scale);
		SetScale(scale);
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

	pComponentXml = a_rParameters.FirstChildElement( "ressourcecomponent" );
	while (pComponentXml)
	{
		AbstractComponent* pComponentObject = ObjectFactory::LoadObject( pComponentXml, this );

		m_vComponents.push_back(pComponentObject);

		//Get the next components
		pComponentXml = pComponentXml->NextSiblingElement( "ressourcecomponent" );
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::_Update( float fFrameTime, float fElapsedTime )
{
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		//Should check if need to be updated??? (think about render and phys)
		(*itComponent)->Update( fFrameTime, fElapsedTime );
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::SetPos( vec3 const& a_vValue )
{
	//Set the component position
	AbstractComponent::SetPos( a_vValue );
	UpdatePos();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::SetScale( float const a_fValue )
{
	//Set the component scale
	AbstractComponent::SetScale( a_fValue );
	UpdateScale();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::SetRotation( vec3 const& a_vValue )
{
	AbstractComponent::SetRotation( a_vValue );
	UpdateRotation();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::UpdatePos()
{
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		pComponent->UpdatePos();
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::UpdateScale()
{
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		pComponent->UpdateScale();
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::UpdateRotation()
{
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		AbstractComponent* pComponent = (*itComponent);
		//The component should not be null
		assert(pComponent);
		pComponent->UpdateRotation();
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