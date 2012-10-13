#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/FirstPersonCamera.h"
#include "BurgerEngine/Core/SphereCamera.h"
#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/MovementHackerComponent.h"
#include "BurgerEngine/Core/RenderComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"

#include "BurgerEngine/Input/EventManager.h"

#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/SkyBox.h"

#include "BurgerEngine/Graphics/DirectionalLight.h"
#include "BurgerEngine/Graphics/SpotLight.h"
#include "BurgerEngine/Graphics/OmniLight.h"
#include "BurgerEngine/Graphics/SpotShadow.h"

#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/MaterialManager.h"
#include "BurgerEngine/Graphics/RenderingContext.h"
#include "BurgerEngine/Graphics/DeferredRenderer.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include "BurgerEngine/Base/CommonBase.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SceneGraph::SceneGraph( const char * pSceneName )
{
	LoadSceneXML( pSceneName );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SceneGraph::~SceneGraph()
{
	Clear();
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SceneGraph::Clear()
{
	FOR_EACH_IT(ComponentCollection, m_vComponentCollection, oComponentIt)
	{
		delete (*oComponentIt);
		(*oComponentIt) = NULL;
	}
	m_vComponentCollection.clear();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SceneGraph::Update( float fFrameTime, float fElapsedTime )
{
	FOR_EACH_IT(ComponentCollection, m_vComponentCollection, oComponentIt)
	{
		//Should check if need to be updated??? (think about render and phys)
		(*oComponentIt)->Update( fFrameTime, fElapsedTime );
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SceneGraph::LoadSceneXML( const char * sName )
{
	TiXmlDocument * pDocument = new TiXmlDocument( sName );

	if(!pDocument->LoadFile())
	{
  		ADD_ERROR_MESSAGE(" Loading : " << pDocument->ErrorDesc());
	}

	//Get the scene
	TiXmlElement * pRoot = pDocument->FirstChildElement( "scene" );

	if( pRoot )
	{
		MeshManager & meshManager = MeshManager::GrabInstance();


		//default settings used if not specified in the xml
		float fFOV = 45.0f, fDofNearBlur = -10.0f, fDofFocalPlane = 15.0f, fDofFarBlur = 150.0f, fDofMaxFarBlur = 1.0f;
		float fPositionSpeed = 0.05f, fRotationSpeed = 0.1f;
		float fToneMappingKey = 0.5f, fGlowMultiplier = 1.0f, fBrightPassThreshold = 1.0f, fBrightPassOffset= 10.0f, fAdaptationBaseTime = 0.9f;
		const char * pColorLUT = NULL;

		TiXmlElement * pXmlObject = pRoot->FirstChildElement( "settings" );

		if( pXmlObject )
		{
			TiXmlElement * pCamera = pXmlObject->FirstChildElement( "camera" );
			if( pCamera )
			{
				std::string sType = pCamera->Attribute("type");

				float x = 0.0f, y = 0.0f, z = 0.0f, rX = 0.0f, rY = 0.0f;
				TiXmlElement * pPosition = pCamera->FirstChildElement( "position" );
				if( pPosition )
				{			
					pPosition->QueryFloatAttribute("x",&x);
					pPosition->QueryFloatAttribute("y",&y);
					pPosition->QueryFloatAttribute("z",&z);

					pPosition->QueryFloatAttribute("rX",&rX);
					pPosition->QueryFloatAttribute("rY",&rY);
				}

				TiXmlElement * pSpeed = pCamera->FirstChildElement( "speed" );
				if( pSpeed )
				{			
					pSpeed->QueryFloatAttribute("position",&fPositionSpeed);
					pSpeed->QueryFloatAttribute("rotation",&fRotationSpeed);
				}
				
				TiXmlElement * pParameters = pCamera->FirstChildElement( "parameters" );
				if( pParameters )
				{			
					pParameters->QueryFloatAttribute("fov",&fFOV);
					pParameters->QueryFloatAttribute("dofNearBlur",&fDofNearBlur);
					pParameters->QueryFloatAttribute("dofFocalPlane",&fDofFocalPlane);
					pParameters->QueryFloatAttribute("dofFarBlur",&fDofFarBlur);
					pParameters->QueryFloatAttribute("dofMaxFarBlur",&fDofMaxFarBlur);
				}

				AbstractCamera* pCamera;
				if( sType == "sphere" )
				{
					float fRadius = 10.0f;
					if( pPosition )
					{
						pPosition->QueryFloatAttribute("radius",&fRadius);
					}
					pCamera = new SphereCamera( fRadius, fFOV, vec3(x,y,z), vec2(rX,rY), vec4( fDofNearBlur, fDofFocalPlane, fDofFarBlur, fDofMaxFarBlur), vec2( fPositionSpeed, fRotationSpeed ) );
				}
				else // sType == "firstperson"
				{
					pCamera = new FirstPersonCamera( fFOV, vec3(x,y,z), vec2(rX,rY), vec4( fDofNearBlur, fDofFocalPlane, fDofFarBlur, fDofMaxFarBlur), vec2( fPositionSpeed, fRotationSpeed ) );
				}

				Engine::GrabInstance().SetCurrentCamera( pCamera );
			}
			else
			{
				Engine::GrabInstance().SetCurrentCamera( new FirstPersonCamera( fFOV, vec3(0.0f,0.0f,0.0f), vec2(0.0f,0.0f), vec4( fDofNearBlur, fDofFocalPlane, fDofFarBlur, fDofMaxFarBlur), vec2( fPositionSpeed, fRotationSpeed ) ) );
			}
			
			TiXmlElement * pPostProcess = pXmlObject->FirstChildElement( "postprocess" );
			if( pPostProcess )
			{
				TiXmlElement * pParameters = pPostProcess->FirstChildElement( "parameters" );
				if( pParameters )
				{			
					pParameters->QueryFloatAttribute("toneMappingKey",&fToneMappingKey);
					pParameters->QueryFloatAttribute("glowMultiplier",&fGlowMultiplier);
					pParameters->QueryFloatAttribute("brightPassThreshold",&fBrightPassThreshold);
					pParameters->QueryFloatAttribute("brightPassOffset",&fBrightPassOffset);
					pParameters->QueryFloatAttribute("adaptationBaseTime",&fAdaptationBaseTime);
					pColorLUT = pParameters->Attribute("colorLUT");
				}
			}
		}
		else
		{
			Engine::GrabInstance().SetCurrentCamera( new FirstPersonCamera( fFOV, vec3(0.0f,0.0f,0.0f), vec2(0.0f,0.0f), vec4( fDofNearBlur, fDofFocalPlane, fDofFarBlur, fDofMaxFarBlur), vec2( fPositionSpeed, fRotationSpeed ) ) );
		}
		
		if( !pColorLUT )
		{
			pColorLUT = "../Data/Textures/xml/neutral_lut.btx.xml";
		}
		Engine::GrabInstance().GrabRenderContext().GrabRenderer().SetPostProcessParameters( fToneMappingKey, fGlowMultiplier, fBrightPassThreshold, fBrightPassOffset, fAdaptationBaseTime, pColorLUT );
		///------------------
		/// We start to load our object as component
		///------------------
		pXmlObject = pRoot->FirstChildElement( "sceneobject" );
		
		while ( pXmlObject )
		{			
			float x, y, z, rX, rY, rZ, scale;
			x = y = z= rX = rY = rZ = 0.0f;
			scale = 1.0f;
			//gets position & rotation
			pXmlObject->QueryFloatAttribute("x",&x);
			pXmlObject->QueryFloatAttribute("y",&y);
			pXmlObject->QueryFloatAttribute("z",&z);

			pXmlObject->QueryFloatAttribute("rX",&rX);
			pXmlObject->QueryFloatAttribute("rY",&rY);
			pXmlObject->QueryFloatAttribute("rZ",&rZ);

			pXmlObject->QueryFloatAttribute("scale",&scale);

			CompositeComponent* pComponent = NULL;
			
			//Check for the current object
			TiXmlElement * pCurrentXmlObject;
			if( pCurrentXmlObject = pXmlObject->FirstChildElement( "ressourcecomponent" ) )
			{
				// Load a component from a xml file
				pComponent = static_cast< CompositeComponent* >( Engine::GrabInstance().GetObjectFactory().LoadObject( pCurrentXmlObject ) );
			}
			else if( pCurrentXmlObject = pXmlObject->FirstChildElement( "component" ) )
			{
				// Load a component written in the scene xml
				pComponent = static_cast< CompositeComponent* >( Engine::GrabInstance().GetObjectFactory().CreateAndInitComponent( *pCurrentXmlObject, NULL ) );
			}
			else if( pCurrentXmlObject = pXmlObject->FirstChildElement( "skybox" ) )
			{
				SkyBox * pSkyBox = new SkyBox( scale );

				TiXmlElement * pXmlMaterial = pCurrentXmlObject->FirstChildElement( "material" );
				if( pXmlMaterial )
				{
					Material * pMaterial = MaterialManager::GrabInstance().addMaterial( pXmlMaterial->GetText() );
					if( pMaterial )
						pSkyBox->SetMaterial( pMaterial );
				}
				Engine::GrabInstance().GrabRenderContext().SetSkyBox( pSkyBox );
			}

			if ( pComponent )
			{
				pComponent->SetScale(scale);
				pComponent->SetRotation(vec3(rX,rY,rZ));
				pComponent->SetPos(vec3(x,y,z));

				//Now every load create an instance
				m_vComponentCollection.push_back(pComponent);
			}

			if(pCurrentXmlObject = pXmlObject->FirstChildElement( "movementhackercomponent" ) )
			{
				TiXmlElement * pParameters;
				if(pParameters = pCurrentXmlObject->FirstChildElement( "parameters" ) )
				{
					MovementHackerComponent* pMovementHackerComponent = new MovementHackerComponent();
					pMovementHackerComponent->Initialize( *pParameters );

					//this is a little bit to "hacky", we assume that the target of the movement hacker is the last created component
					//we might use some kind of ID the identify the component
					pMovementHackerComponent->SetComponent( m_vComponentCollection.back() );
					
					m_vComponentCollection.push_back(pMovementHackerComponent);
				}
			}



			//moves on to the next object
			pXmlObject = pXmlObject->NextSiblingElement( "sceneobject" );

		}
	}
}

