#include "BurgerEngine/Core/ParticleComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/FX/ParticleContext.h"
#include "BurgerEngine/fx/ParticleSystem.h"
#include "BurgerEngine/fx/ParticleGroup.h"
#include "BurgerEngine/fx/ParticleAccelerationEffector.h"
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
	//Retrieve position
	//First we should check is there is not an desc offset
	//to the parent node
	/*float x, y, z, rX, rY, rZ, scale;

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

		pPositionXml->QueryFloatAttribute("scale",&scale);
	}

	//Find a way to retrieve position from the composite
	TiXmlElement const* pMeshXml = a_rParameters.FirstChildElement( "mesh" );
	if(pMeshXml)
	{
		//checks for the filename
		TiXmlElement const* pFilename = pMeshXml->FirstChildElement( "file" );
		if(pFilename)
		{
			std::string sMeshFileName( pFilename->GetText() );
			StaticMesh* pMesh = MeshManager::GrabInstance().loadMesh( sMeshFileName );

			if( pMesh )
			{
				m_pMesh = new SceneMesh( pMesh );

				vec3 f3OffsetedPos(x,y,z);
				SetPos(f3OffsetedPos);
				//Get the parent node transformation
				CompositeComponent const* pParent = GetParent();
				if (pParent)
				{
					f3OffsetedPos += pParent->GetPos();
					//pSceneMesh->SetPos( pParent->GetPos());
				}

				m_pMesh->SetPos(f3OffsetedPos);
				m_pMesh->SetRotation( vec3( rX, rY, rZ ));
				m_pMesh->SetScale( scale);
				m_pMesh->ComputeBoundingBox();

				//checks for materials used on different parts of the mesh
				unsigned int iPartCount = 0;

				TiXmlElement const* pPart = pMeshXml->FirstChildElement( "part" );
				while( pPart )
				{
					++iPartCount;
					TiXmlElement const* pXmlMaterial = pPart->FirstChildElement( "material" );
					if( pXmlMaterial )
					{
						Material * pMaterial = MaterialManager::GrabInstance().addMaterial( pXmlMaterial->GetText() );
						if( pMaterial )
						{
							m_pMesh->AddMaterial( pMaterial );
						}
					}			

					pPart = pPart->NextSiblingElement( "part" );
				}

				m_pMesh->SetPartCount( iPartCount );

				//Add itsefl to the render context
				//Or maybe juste the mesh should register?? Or To the rendering pipeline( which does not exist so far)
				Engine::GrabInstance().GrabRenderContext().AddMesh(*m_pMesh);
			}

		}

	}*/
	m_pSystem = new ParticleSystem();



	if (m_pSystem)
	{
		/*
			TestValue
		*/
		/// Create the matos
		//Material * pMaterial = MaterialManager::GrabInstance().addMaterial( pXmlMaterial->GetText() );
		Material * pMaterial = MaterialManager::GrabInstance().addMaterial( "../Data/Materials/prison/ParticleTest.bma.xml" );
		/// \todo Creat a Parameter struc for Emitter init
		ParticleGroup* pGroup = new ParticleGroup();
		pGroup->Initialize();
		ParticleEmitter& rEmitter = pGroup->GrabEmitter();
		rEmitter.m_fEmissionFrequency = 40.0f;
		rEmitter.m_uMaxCount = 10000;
		rEmitter.m_fLife = 10.0f;
		rEmitter.m_uLoopCount = 0;

		ParticleAccelerationEffector* pEffector = new ParticleAccelerationEffector();
		pEffector->SetAcceleration(vec3(0.0,-10.0,0.0));
		pGroup->AddEffector(*pEffector);
		pGroup->SetMaterial(pMaterial);
		m_pSystem->AddGroup(pGroup);
		m_pSystem->Play();


		ParticleContext& rContext = Engine::GrabInstance().GrabParticleContext();
		rContext.RegisterFXInstance(*m_pSystem);
	}

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleComponent::_Update( float fFrameTime, float fElapsedTime )
{
}

