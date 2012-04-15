#include "BurgerEngine/Core/ParticleComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/FX/ParticleContext.h"
#include "BurgerEngine/fx/ParticleSystem.h"
#include "BurgerEngine/fx/ParticleGroup.h"
#include "BurgerEngine/fx/ParticleAccelerationEffector.h"
#include "BurgerEngine/fx/ParticleLifeEffector.h"
#include "BurgerEngine/fx/ParticleVelocityEffector.h"
#include "BurgerEngine/Graphics/MaterialManager.h"


#include "BurgerEngine/External/TinyXml/TinyXml.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
ParticleComponent::ParticleComponent(CompositeComponent* a_pParent):
	AbstractComponent(PARTICLE, a_pParent),
	m_pSystem(NULL)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
ParticleComponent::ParticleComponent(AbstractComponent const& a_rToCopy):
	AbstractComponent(PARTICLE),
	m_pSystem(NULL)
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
ParticleComponent::~ParticleComponent()
{
	m_pSystem->Terminate();
	delete m_pSystem;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleComponent::Initialize(TiXmlElement const& a_rParameters)
{
	//Create the whole system
	m_pSystem = new ParticleSystem();

	TiXmlElement const* pGroupsXml = a_rParameters.FirstChildElement( "group" );

	while (pGroupsXml)
	{
		ParticleGroup* pGroup = new ParticleGroup();
		/// \todo Creat a Parameter struc for Emitter init
		pGroup->Initialize();

		//Retrieve position
		//First we should check is there is not an desc offset
		//to the parent node
		float x, y, z;

		pGroupsXml->QueryFloatAttribute("x",&x);
		pGroupsXml->QueryFloatAttribute("y",&y);
		pGroupsXml->QueryFloatAttribute("z",&z);

		TiXmlElement const* pEmitterXml = pGroupsXml->FirstChildElement( "emitter" );
		if (pEmitterXml)
		{
			// TODO : take position
			ParticleEmitter& rEmitter = pGroup->GrabEmitter();
			pEmitterXml->QueryFloatAttribute("frequency",&rEmitter.m_fEmissionFrequency);
			pEmitterXml->QueryIntAttribute("max_count",(int*)&rEmitter.m_uMaxCount);
			pEmitterXml->QueryIntAttribute("loop_count",(int*)&rEmitter.m_uLoopCount);
			pEmitterXml->QueryFloatAttribute("life",&rEmitter.m_fLife);
		}
		
		TiXmlElement const* pEffectorXml = pGroupsXml->FirstChildElement( "effector" );
		//now were adding the effector for each row 
		while (pEffectorXml)
		{
			std::string sId;
			pEffectorXml->QueryValueAttribute<std::string>("id",&sId);
			ParticleEffector* pEffector;
			if (sId == "velocity")
			{
				ParticleVelocityEffector* pTypedEffector = new ParticleVelocityEffector();
				vec3 vMax, vMin;
				TiXmlElement const* pMinXml = pEffectorXml->FirstChildElement( "min" );
				pMinXml->QueryFloatAttribute("x", &vMin.x);
				pMinXml->QueryFloatAttribute("y", &vMin.y);
				pMinXml->QueryFloatAttribute("z", &vMin.z);

				TiXmlElement const* pMaxXml = pEffectorXml->FirstChildElement( "max" );
				pMaxXml->QueryFloatAttribute("x", &vMax.x);
				pMaxXml->QueryFloatAttribute("y", &vMax.y);
				pMaxXml->QueryFloatAttribute("z", &vMax.z);
				//pEffector->SetVelocity(vec3(-5.0,20.0,-5.0), vec3(5.0,15.0,5.0));
				pTypedEffector->SetVelocity(vMin, vMax);
				pEffector = pTypedEffector;
			}
			else if (sId == "life")
			{
				ParticleLifeEffector* pTypedEffector = new ParticleLifeEffector();
				float fMin, fMax;
				TiXmlElement const* pValueXml = pEffectorXml->FirstChildElement( "value" );
				pValueXml->QueryFloatAttribute("min", &fMin);
				pValueXml->QueryFloatAttribute("max", &fMax);

				pTypedEffector->SetLifeTime(fMin, fMax);
				pEffector = pTypedEffector;
			}
			else if (sId == "acceleration")
			{
				ParticleAccelerationEffector* pTypedEffector = new ParticleAccelerationEffector();
				vec3 vMax, vMin;
				TiXmlElement const* pMinXml = pEffectorXml->FirstChildElement( "min" );
				pMinXml->QueryFloatAttribute("x", &vMin.x);
				pMinXml->QueryFloatAttribute("y", &vMin.y);
				pMinXml->QueryFloatAttribute("z", &vMin.z);

				TiXmlElement const* pMaxXml = pEffectorXml->FirstChildElement( "max" );
				pMaxXml->QueryFloatAttribute("x", &vMax.x);
				pMaxXml->QueryFloatAttribute("y", &vMax.y);
				pMaxXml->QueryFloatAttribute("z", &vMax.z);
				pTypedEffector->SetAcceleration(vMin, vMax);
				pEffector = pTypedEffector;
			}
			pGroup->AddEffector(*pEffector);

			//
			pEffectorXml = pEffectorXml->NextSiblingElement( "effector" );
		}
		//The material 
		TiXmlElement const* pMaterialXml = pGroupsXml->FirstChildElement( "material" );
		Material * pMaterial = MaterialManager::GrabInstance().addMaterial( pMaterialXml->GetText() );
		pGroup->SetMaterial(pMaterial);

		m_pSystem->AddGroup(pGroup);
		//Get the next group
		pGroupsXml = pGroupsXml->NextSiblingElement( "group" );
	}
	
	m_pSystem->Play();
	ParticleContext& rContext = Engine::GrabInstance().GrabParticleContext();
	rContext.RegisterFXInstance(*m_pSystem);
	

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleComponent::_Update( float fFrameTime, float fElapsedTime )
{
}

