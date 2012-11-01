#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/RenderComponent.h"
#include "BurgerEngine/Core/LightComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include "BurgerEngine/Core/Engine.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::CompositeComponent(CompositeComponent* a_pParent)
	: AbstractComponent(a_pParent)
{
	AbstractComponent::SetPos(vec3(0,0,0));
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::CompositeComponent(AbstractComponent const& a_rToCopy):
	AbstractComponent()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
CompositeComponent::~CompositeComponent()
{
	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		delete (*itComponent);
		(*itComponent) = NULL;
	}
	m_vComponents.clear();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::Initialize(TiXmlElement const& a_rParameters)
{
	//these values might be overwritten if the component :
	// - is the first child of a sceneobject in a scene
	// - is loaded as a ressource component by another component and this ressource component has a position tag
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
		AbstractComponent* pComponentObject = Engine::GrabInstance().GetObjectFactory().CreateAndInitComponent(*pComponentXml, this);

		m_vComponents.push_back(pComponentObject);

		//Get the next components
		pComponentXml = pComponentXml->NextSiblingElement( "component" );
	}

	pComponentXml = a_rParameters.FirstChildElement( "ressourcecomponent" );
	while (pComponentXml)
	{
		AbstractComponent* pComponentObject = Engine::GrabInstance().GetObjectFactory().LoadObject( pComponentXml, this );

		m_vComponents.push_back(pComponentObject);

		//Get the next components
		pComponentXml = pComponentXml->NextSiblingElement( "ressourcecomponent" );
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::Update( float fFrameTime, float fElapsedTime )
{

	FOR_EACH_IT(std::vector<AbstractComponent*>, m_vComponents, itComponent)
	{
		//Should check if need to be updated??? (think about render and phys)
		(*itComponent)->SetUpdateNeeded(m_bUpdateNeeded);
		(*itComponent)->Update( fFrameTime, fElapsedTime );
	}
	m_bUpdateNeeded = false;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::SetPos( vec3 const& a_vValue )
{
	//Set the component position
	AbstractComponent::SetPos( a_vValue );
	m_bUpdateNeeded = true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::SetScale( float const a_fValue )
{
	//Set the component scale
	AbstractComponent::SetScale( a_fValue );
	m_bUpdateNeeded = true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CompositeComponent::SetRotation( vec3 const& a_vValue )
{
	AbstractComponent::SetRotation( a_vValue );
	m_bUpdateNeeded = true;
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