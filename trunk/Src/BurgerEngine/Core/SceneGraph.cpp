#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/Engine.h"

#include "BurgerEngine/Core/FirstPersonCamera.h"
#include "BurgerEngine/Core/SphereCamera.h"

#include "BurgerEngine/Core/CompositeComponent.h"
#include "BurgerEngine/Core/RenderComponent.h"

#include "BurgerEngine/Input/EventManager.h"

#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/SkyBox.h"

#include "BurgerEngine/Graphics/SceneLight.h"
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
	m_oFactory.Init();
	LoadSceneXML( pSceneName );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SceneGraph::~SceneGraph()
{
	m_oFactory.Terminate();
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
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SceneGraph::Update()
{
	FOR_EACH_IT(ComponentCollection, m_vComponentCollection, oComponentIt)
	{
		//Should check if need to be updated??? (think about render and phys)
		(*oComponentIt)->Update();
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
				const char * pColorLUT = NULL;
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

				if( !pColorLUT )
				{
					pColorLUT = "../Data/Textures/xml/neutral_lut.btx.xml";
				}
				Engine::GrabInstance().GrabRenderContext().GrabRenderer().SetPostProcessParameters( fToneMappingKey, fGlowMultiplier, fBrightPassThreshold, fBrightPassOffset, fAdaptationBaseTime, pColorLUT );
			}
			else
			{
				Engine::GrabInstance().GrabRenderContext().GrabRenderer().SetPostProcessParameters( fToneMappingKey, fGlowMultiplier, fBrightPassThreshold, fBrightPassOffset, fAdaptationBaseTime, "../Data/Textures/xml/neutral_lut.btx.xml" );
			}
			
		}
		else
		{
			Engine::GrabInstance().SetCurrentCamera( new FirstPersonCamera( fFOV, vec3(0.0f,0.0f,0.0f), vec2(0.0f,0.0f), vec4( fDofNearBlur, fDofFocalPlane, fDofFarBlur, fDofMaxFarBlur), vec2( fPositionSpeed, fRotationSpeed ) ) );
			Engine::GrabInstance().GrabRenderContext().GrabRenderer().SetPostProcessParameters( fToneMappingKey, fGlowMultiplier, fBrightPassThreshold, fBrightPassOffset, fAdaptationBaseTime, "../Data/Textures/xml/neutral_lut.btx.xml" );
		}

		///------------------
		/// We start to load our object as component
		///------------------
		pXmlObject = pRoot->FirstChildElement( "sceneobject" );
		
		while ( pXmlObject )
		{			
			float x, y, z, rX, rY, rZ, scale;
			
			//gets position & rotation
			pXmlObject->QueryFloatAttribute("x",&x);
			pXmlObject->QueryFloatAttribute("y",&y);
			pXmlObject->QueryFloatAttribute("z",&z);

			pXmlObject->QueryFloatAttribute("rX",&rX);
			pXmlObject->QueryFloatAttribute("rY",&rY);
			pXmlObject->QueryFloatAttribute("rZ",&rZ);

			pXmlObject->QueryFloatAttribute("scale",&scale);


			//Check for the current object
			// Right now only static mesh are suported by the component system
			TiXmlElement * pCurrentXmlObject;
			if( pCurrentXmlObject = pXmlObject->FirstChildElement( "ressourcecomponent" ) )
			{
				// What is the component file
				CompositeComponent* pComponent = NULL;
				pComponent = m_oFactory.LoadObject(pCurrentXmlObject->GetText());
				
				// Add the instance position to the root component
				// and the ortation and scale
				// the problem here is that we should override every
				// instance proprety, like skin color for a player etc...
				if (pComponent != NULL)
				{
					pComponent->SetScale(scale);
					pComponent->SetRotation(vec3(rX,rY,rZ));
					pComponent->SetPos(vec3(x,y,z));
					
					
					/// HACK : we set directly the position/rot/scale to the mesh
					RenderComponent* pRenderComponent = static_cast<RenderComponent*>(pComponent->TryGrabComponentByType(RENDER));
					if (pRenderComponent)
					{
						//pRenderComponent->SetPos(vec3(x,y,z));
						//pRenderComponent->GrabInternalMesh().SetPos(vec3(x,y,z));
						//pRenderComponent->GrabInternalMesh().SetRotation(vec3(rX,rY,rZ));
						//pRenderComponent->GrabInternalMesh().SetScale(scale);
					}
				}

				//Ad instance name

				//Add directly to the graph, this might change
				//Now every load create an instance
				m_vComponentCollection.push_back(pComponent);
			}
			else if( pCurrentXmlObject = pXmlObject->FirstChildElement( "skybox" ) )
			{
				/*TiXmlElement * pXmlMaterial = pCurrentXmlObject->FirstChildElement( "material" );
				m_pSkyBox = new SkyBox( scale );
				if( pXmlMaterial )
				{
					Material * pMaterial = MaterialManager::GrabInstance().addMaterial( pXmlMaterial->GetText() );
					if( pMaterial )
						m_pSkyBox->SetMaterial( pMaterial );
				}*/
			}

			//moves on to the next object
			pXmlObject = pXmlObject->NextSiblingElement( "sceneobject" );

		}
	}
}

