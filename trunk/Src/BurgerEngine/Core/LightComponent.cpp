#include "BurgerEngine/Core/LightComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/CompositeComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include "BurgerEngine/Graphics/SpotShadow.h"
#include "BurgerEngine/Graphics/SpotLight.h"
#include "BurgerEngine/Graphics/OmniLight.h"
#include "BurgerEngine/Graphics/RenderingContext.h"




//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
LightComponent::LightComponent(CompositeComponent* a_pParent):
AbstractComponent(LIGHT, a_pParent),
	m_pLight(NULL)
{

	m_mStringToLightTypeMap["omni"] = SceneLight::E_OMNI_LIGHT;
	m_mStringToLightTypeMap["spot"] = SceneLight::E_SPOT_LIGHT;
	m_mStringToLightTypeMap["spotshadow"] = SceneLight::E_SPOT_SHADOW;
	m_mStringToLightTypeMap["directional"] = SceneLight::E_DIRECTIONAL;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
LightComponent::LightComponent(AbstractComponent const& a_rToCopy):
AbstractComponent(LIGHT)
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
LightComponent::~LightComponent()
{
	//Maybe will be deleted by someone else
	delete m_pLight;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void LightComponent::Initialize(TiXmlElement const& a_rParameters)
{
	TiXmlElement const* pXmlElement;

	//Retrieve position
	//First we should check is there is not an desc offset
	//to the parent node
	float x, y, z, rX, rY, rZ;
	TiXmlElement const* pPositionXml = a_rParameters.FirstChildElement( "position" );
	if(pPositionXml)
	{
		//gets position & bounded volume information 
		pPositionXml->QueryFloatAttribute("x",&x);
		pPositionXml->QueryFloatAttribute("y",&y);
		pPositionXml->QueryFloatAttribute("z",&z);

		pPositionXml->QueryFloatAttribute("rX",&rX);
		pPositionXml->QueryFloatAttribute("rY",&rY);
		pPositionXml->QueryFloatAttribute("rZ",&rZ);
	}

	float r, g, b;
	pXmlElement = a_rParameters.FirstChildElement("Color");
	if( pXmlElement )
	{
		pXmlElement->QueryFloatAttribute("r",&r);
		pXmlElement->QueryFloatAttribute("g",&g);
		pXmlElement->QueryFloatAttribute("b",&b);
	}

	float fMultiplier;
	pXmlElement = a_rParameters.FirstChildElement("Multiplier");
	if( pXmlElement )
	{
		pXmlElement->QueryFloatAttribute("value",&fMultiplier);
	}

	pXmlElement = a_rParameters.FirstChildElement("light");
	SceneLight::LightType eType;
	if (pXmlElement)
	{
		std::string sLightType;
		pXmlElement->QueryValueAttribute<std::string>("type",&sLightType);
		eType = m_mStringToLightTypeMap[ sLightType ];
	}
	

	if( (eType & SceneLight::E_OMNI_LIGHT) == SceneLight::E_OMNI_LIGHT )
	{
		float fRadius;
		pXmlElement = a_rParameters.FirstChildElement("Radius");
		if( pXmlElement )
		{
			pXmlElement->QueryFloatAttribute("value",&fRadius);
		}
		if( (eType & SceneLight::E_SPOT_LIGHT) == SceneLight::E_SPOT_LIGHT )
		{
			pXmlElement = a_rParameters.FirstChildElement("InnerAngle");
			float fInnerAngle;
			if( pXmlElement )
			{
				pXmlElement->QueryFloatAttribute("value",&fInnerAngle);

			}

			pXmlElement = a_rParameters.FirstChildElement("OuterAngle");
			float fOuterAngle;
			if( pXmlElement )
			{
				pXmlElement->QueryFloatAttribute("value",&fOuterAngle);
			}

			if( (eType & SceneLight::E_SPOT_SHADOW) == SceneLight::E_SPOT_SHADOW )
			{
				m_pLight = new SpotShadow();	
			}
			else
			{
				m_pLight = new SpotLight();
			}

			m_pLight->SetRotation( vec3( rX,rY, 0.0 ) );
			static_cast< SpotLight* >(m_pLight)->SetOuterAngle( fOuterAngle );
			static_cast< SpotLight* >(m_pLight)->SetInnerAngle( fInnerAngle );
		}
		else
		{
			m_pLight = new OmniLight();
		}

		m_pLight->SetColor( vec3( r, g, b ) );
		m_pLight->SetMultiplier( fMultiplier );
		static_cast< OmniLight* >(m_pLight)->SetRadius( fRadius );
		m_pLight->SetPos( vec3( x, y, z ) );

		if( (eType & SceneLight::E_SPOT_LIGHT) == SceneLight::E_SPOT_LIGHT )
		{
			static_cast< SpotLight* >(m_pLight)->ComputeBoundingBox();
		}
	}
	else if( ( eType & SceneLight::E_DIRECTIONAL) == SceneLight::E_DIRECTIONAL )
	{
		m_pLight = new SceneLight();
		m_pLight->SetColor( vec3( r, g, b ) );
		m_pLight->SetMultiplier( fMultiplier );
		m_pLight->SetPos( vec3( x, y, z ) );
	}

	SetPos(vec3( x, y, z ));
	//Add the light to the render context
	Engine::GrabInstance().GrabRenderContext().AddLight(*m_pLight, eType);
	m_pLight->SetType(eType);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void LightComponent::_Update()
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void LightComponent::UpdatePos()
{
	//Get the parent node transformation
	CompositeComponent const* pParent = GetParent();
	if (pParent)
	{
		m_pLight->SetPos(GetPos() + pParent->GetPos()); 
	}
	else
	{
		m_pLight->SetPos(GetPos());
	}

	if( (m_pLight->GetType() & SceneLight::E_SPOT_LIGHT) == SceneLight::E_SPOT_LIGHT )
	{
		static_cast< SpotLight* >(m_pLight)->ComputeBoundingBox();
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void LightComponent::UpdateRotation(vec3 const& a_rf3Rotation)
{
	m_pLight->SetRotation(a_rf3Rotation);
	if( (m_pLight->GetType() & SceneLight::E_SPOT_LIGHT) == SceneLight::E_SPOT_LIGHT )
	{
		static_cast< SpotLight* >(m_pLight)->ComputeBoundingBox();
	}
}
