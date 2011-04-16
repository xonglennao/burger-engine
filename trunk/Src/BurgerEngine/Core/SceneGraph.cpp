#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"
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
	/// \TEMP FRANCK : retrieve light vector 
	std::vector< SpotShadow* >& rShadowVector = Engine::GrabInstance().GrabRenderContext().GrabSpotShadows();
	std::vector< SpotLight* >& rSpotVector = Engine::GrabInstance().GrabRenderContext().GrabSpotLights();
	std::vector< SceneLight* >& rDirectionalVector = Engine::GrabInstance().GrabRenderContext().GrabDirectionalLights();

	/// \temp also
	std::map< std::string, SceneLight::LightType > mStringToLightTypeMap;
	mStringToLightTypeMap["omni"] = SceneLight::E_OMNI_LIGHT;
	mStringToLightTypeMap["spot"] = SceneLight::E_SPOT_LIGHT;
	mStringToLightTypeMap["spotshadow"] = SceneLight::E_SPOT_SHADOW;
	mStringToLightTypeMap["directional"] = SceneLight::E_DIRECTIONAL;


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
		TiXmlElement * pXmlObject = pRoot->FirstChildElement( "sceneobject" );
		
		while ( pXmlObject )
		{			
			float x, y, z, rX, rY, rZ, scale;
			
			//gets position & bounded volume information 
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
			// \todo make light as component
			else if( pCurrentXmlObject = pXmlObject->FirstChildElement( "light" ) )
			{
				TiXmlElement * pXmlElement;
				float r, g, b;
				pXmlElement = pCurrentXmlObject->FirstChildElement("Color");
				if( pXmlElement )
				{
					pXmlElement->QueryFloatAttribute("r",&r);
					pXmlElement->QueryFloatAttribute("g",&g);
					pXmlElement->QueryFloatAttribute("b",&b);
				}
					
				float fMultiplier;
				pXmlElement = pCurrentXmlObject->FirstChildElement("Multiplier");
				if( pXmlElement )
				{
					pXmlElement->QueryFloatAttribute("value",&fMultiplier);
				}

				std::string sType = pCurrentXmlObject->Attribute("type");
				SceneLight::LightType eType = mStringToLightTypeMap[ sType ];

				SceneLight * pSceneLight;
				if( (eType & SceneLight::E_OMNI_LIGHT) == SceneLight::E_OMNI_LIGHT )
				{
					float fRadius;
					pXmlElement = pCurrentXmlObject->FirstChildElement("Radius");
					if( pXmlElement )
					{
						pXmlElement->QueryFloatAttribute("value",&fRadius);
					}
					if( (eType & SceneLight::E_SPOT_LIGHT) == SceneLight::E_SPOT_LIGHT )
					{
						pXmlElement = pCurrentXmlObject->FirstChildElement("InnerAngle");
						float fInnerAngle;
						if( pXmlElement )
						{
							pXmlElement->QueryFloatAttribute("value",&fInnerAngle);

						}

						pXmlElement = pCurrentXmlObject->FirstChildElement("OuterAngle");
						float fOuterAngle;
						if( pXmlElement )
						{
							pXmlElement->QueryFloatAttribute("value",&fOuterAngle);
						}

						if( (eType & SceneLight::E_SPOT_SHADOW) == SceneLight::E_SPOT_SHADOW )
						{
							pSceneLight = new SpotShadow();	
							rShadowVector.push_back( static_cast< SpotShadow* >(pSceneLight) );
						}
						else
						{
							pSceneLight = new SpotLight();
							rSpotVector.push_back( static_cast< SpotLight* >(pSceneLight) );
						}

						pSceneLight->SetRotation( vec3( rX,rY, 0.0 ) );
						static_cast< SpotLight* >(pSceneLight)->SetOuterAngle( fOuterAngle );
						static_cast< SpotLight* >(pSceneLight)->SetInnerAngle( fInnerAngle );
					}
					else
					{
						pSceneLight = new OmniLight();
						Engine::GrabInstance().GrabRenderContext().AddOmniLight(static_cast< OmniLight& >(*pSceneLight));
					}

					pSceneLight->SetColor( vec3( r, g, b ) );
					pSceneLight->SetMultiplier( fMultiplier );
					static_cast< OmniLight* >(pSceneLight)->SetRadius( fRadius );
					pSceneLight->SetPos( vec3( x, y, z ) );
						
					if( (eType & SceneLight::E_SPOT_LIGHT) == SceneLight::E_SPOT_LIGHT )
					{
						static_cast< SpotLight* >(pSceneLight)->ComputeBoundingBox();
					}
				}
				else if( ( eType & SceneLight::E_DIRECTIONAL) == SceneLight::E_DIRECTIONAL )
				{
					pSceneLight = new SceneLight();
					pSceneLight->SetColor( vec3( r, g, b ) );
					pSceneLight->SetMultiplier( fMultiplier );
					pSceneLight->SetPos( vec3( x, y, z ) );
					rDirectionalVector.push_back( pSceneLight );
				}
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

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SceneGraph::_LoadComponentsXML( const char * sName )
{
	/*TiXmlDocument * pDocument = new TiXmlDocument( sName );

	if(!pDocument->LoadFile())
	{
		std::cerr << "[_LoadComponentsXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
	}

	TiXmlElement * pRoot = pDocument->FirstChildElement("component_ressource");

	if( pRoot )
	{
		//We are going to ADD all the components ressource (commons) to the ObjectFactory
		TiXmlElement * pXmlObject = pRoot->FirstChildElement("object");

		while ( pXmlObject )
		{
			float x, y, z, rX, rY, rZ, scale;

			//gets position & bounded volume information 
			pXmlObject->QueryFloatAttribute("x",&x);
			pXmlObject->QueryFloatAttribute("y",&y);
			pXmlObject->QueryFloatAttribute("z",&z);

			pXmlObject->QueryFloatAttribute("rX",&rX);
			pXmlObject->QueryFloatAttribute("rY",&rY);
			pXmlObject->QueryFloatAttribute("rZ",&rZ);

			pXmlObject->QueryFloatAttribute("scale",&scale);



			//checks if the current sceneobject is a mesh
			TiXmlElement * pCurrentXmlObject;// = pXmlObject->FirstChildElement( "mesh" );
			if( pCurrentXmlObject = pXmlObject->FirstChildElement( "mesh" ) )
			{
				//checks for the filename
				TiXmlElement * pXmlPoint = pCurrentXmlObject->FirstChildElement( "file" );
				if( pXmlPoint )
				{
					std::string sMeshFileName( pXmlPoint->GetText() );
					StaticMesh* pMesh = meshManager.loadMesh( sMeshFileName );

					if( pMesh )
					{
						SceneMesh * pSceneMesh = new SceneMesh( pMesh );

						pSceneMesh->SetPos( vec3( x, y, z ) );
						pSceneMesh->SetRotation( vec3( rX, rY, rZ ));
						pSceneMesh->SetScale( scale );

						//checks for materials used on different parts of the mesh
						unsigned int iPartCount = 0;

						pXmlPoint = pCurrentXmlObject->FirstChildElement( "part" );
						while( pXmlPoint )
						{
							++iPartCount;
							TiXmlElement * pXmlMaterial = pXmlPoint->FirstChildElement( "material" );
							if( pXmlMaterial )
							{
								Material * pMaterial = MaterialManager::GrabInstance().addMaterial( pXmlMaterial->GetText() );
								if( pMaterial )
									pSceneMesh->AddMaterial( pMaterial );
							}			

							pXmlPoint = pXmlPoint->NextSiblingElement( "part" );
						}

						pSceneMesh->SetPartCount( iPartCount );
						if( pSceneMesh->IsOpaque() )
						{
							m_oSceneMeshes.push_back( pSceneMesh );
						}
						else if( pSceneMesh->IsTransparent() )
						{
							m_oTransparentSceneMeshes.push_back( pSceneMesh );
						}
					}

				}

			}
		}

	}*/
}
