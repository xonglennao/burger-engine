#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"

#include "BurgerEngine/Input/EventManager.h"

#include "BurgerEngine/Graphics/SceneMesh.h"

#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/Graphics/SpotLight.h"
#include "BurgerEngine/Graphics/OmniLight.h"
#include "BurgerEngine/Graphics/SpotShadow.h"

#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/MaterialManager.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include "BurgerEngine/Base/CommonBase.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SceneGraph::SceneGraph()
{
	m_eStringToLightTypeMap["omni"] = SceneLight::E_OMNI_LIGHT;
	m_eStringToLightTypeMap["spot"] = SceneLight::E_SPOT_LIGHT;
	m_eStringToLightTypeMap["spotshadow"] = SceneLight::E_SPOT_SHADOW;
	m_eStringToLightTypeMap["directional"] = SceneLight::E_DIRECTIONAL;

	LoadSceneXML( "../Data/Scenes/test_transparency.xml" );
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
	std::vector< SceneMesh* >::iterator oMeshIt = m_oSceneMeshes.begin();
	while( oMeshIt != m_oSceneMeshes.end() )
	{
		delete (*oMeshIt);
		(*oMeshIt) = NULL;
		++oMeshIt;
	}

	std::vector< OmniLight* >::iterator oLightIt = m_oOmniLights.begin();
	while( oLightIt != m_oOmniLights.end() )
	{
		delete (*oLightIt);
		(*oLightIt) = NULL;
		++oLightIt;
	}

	std::vector< SpotLight* >::iterator oSpotLightIt = m_oSpotLights.begin();
	while( oSpotLightIt != m_oSpotLights.end() )
	{
		delete (*oSpotLightIt);
		(*oSpotLightIt) = NULL;
		++oSpotLightIt;
	}

	std::vector< SpotShadow* >::iterator oSpotShadowIt = m_oSpotShadows.begin();
	while( oSpotShadowIt != m_oSpotShadows.end() )
	{
		delete (*oSpotShadowIt);
		(*oSpotShadowIt) = NULL;
		++oSpotShadowIt;
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
  		std::cerr << "[ReadXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
	}
	
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

			//checks if the current sceneobject is a mesh
			TiXmlElement * pXmlMesh = pXmlObject->FirstChildElement( "mesh" );
			if( pXmlMesh )
			{
				//checks for the filename
				TiXmlElement * pXmlPoint = pXmlMesh->FirstChildElement( "file" );
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

						pXmlPoint = pXmlMesh->FirstChildElement( "part" );
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
					}

				}

			}
			else
			{
				TiXmlElement * pXmlLight = pXmlObject->FirstChildElement( "light" );
				if( pXmlLight )
				{
					TiXmlElement * pXmlElement;

					float r, g, b;
					pXmlElement = pXmlLight->FirstChildElement("Color");
					if( pXmlElement )
					{
						pXmlElement->QueryFloatAttribute("r",&r);
						pXmlElement->QueryFloatAttribute("g",&g);
						pXmlElement->QueryFloatAttribute("b",&b);
					}
					
					float fMultiplier;
					pXmlElement = pXmlLight->FirstChildElement("Multiplier");
					if( pXmlElement )
					{
						pXmlElement->QueryFloatAttribute("value",&fMultiplier);
					}

					std::string sType = pXmlLight->Attribute("type");
					SceneLight::LightType eType = m_eStringToLightTypeMap[ sType ];

					SceneLight * pSceneLight;
					if( (eType & SceneLight::E_OMNI_LIGHT) == SceneLight::E_OMNI_LIGHT )
					{
						float fRadius;
						pXmlElement = pXmlLight->FirstChildElement("Radius");
						if( pXmlElement )
						{
							pXmlElement->QueryFloatAttribute("value",&fRadius);
						}
						if( (eType & SceneLight::E_SPOT_LIGHT) == SceneLight::E_SPOT_LIGHT )
						{
							pXmlElement = pXmlLight->FirstChildElement("InnerAngle");
							float fInnerAngle;
							if( pXmlElement )
							{
								pXmlElement->QueryFloatAttribute("value",&fInnerAngle);

							}

							pXmlElement = pXmlLight->FirstChildElement("OuterAngle");
							float fOuterAngle;
							if( pXmlElement )
							{
								pXmlElement->QueryFloatAttribute("value",&fOuterAngle);
							}

							if( (eType & SceneLight::E_SPOT_SHADOW) == SceneLight::E_SPOT_SHADOW )
							{
								pSceneLight = new SpotShadow();	
								m_oSpotShadows.push_back( static_cast< SpotShadow* >(pSceneLight) );
							}
							else
							{
								pSceneLight = new SpotLight();
								m_oSpotLights.push_back( static_cast< SpotLight* >(pSceneLight) );
							}

							pSceneLight->SetRotation( vec3( rX,rY, 0.0 ) );
							static_cast< SpotLight* >(pSceneLight)->SetOuterAngle( fOuterAngle );
							static_cast< SpotLight* >(pSceneLight)->SetInnerAngle( fInnerAngle );
						}
						else
						{
							pSceneLight = new OmniLight();
							m_oOmniLights.push_back( static_cast< OmniLight* >(pSceneLight) );
						}

						pSceneLight->SetColor( vec3( r, g, b ) );
						pSceneLight->SetMultiplier( fMultiplier );
						static_cast< OmniLight* >(pSceneLight)->SetRadius( fRadius );
						pSceneLight->SetPos( vec3( x, y, z ) );
						
						if( (eType & SceneLight::E_SPOT_LIGHT) == SceneLight::E_SPOT_LIGHT )
						{
							static_cast< SpotLight* >(pSceneLight)->ComputeBoundingVolume();
						}
					}
				}
			}
			//moves on to the next object
			pXmlObject = pXmlObject->NextSiblingElement( "sceneobject" );
		}

		/*//check if there's water on the level
		pXmlObject = pRoot->FirstChildElement( "water" );
		if( pXmlObject )
		{
			float fWaterLevel;
			pXmlObject->QueryFloatAttribute("level",&fWaterLevel);
		}*/

	}
}
