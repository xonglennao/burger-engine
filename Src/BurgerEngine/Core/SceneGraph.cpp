#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"

#include "BurgerEngine/Input/EventManager.h"

#include "BurgerEngine/Graphics/SceneMesh.h"

#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/Graphics/SpotLight.h"
#include "BurgerEngine/Graphics/OmniLight.h"

#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/MaterialManager.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include <iostream>

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SceneGraph::SceneGraph()
{
	LoadSceneXML( "../Data/Scenes/testSpots.xml" );
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
						
					if( pMesh ){

						SceneMesh * pSceneMesh = new SceneMesh( pMesh );

						pSceneMesh->SetPos( vec3( x, y, z ) );
						pSceneMesh->SetRotation( vec3( rX, rY, rZ ));
						pSceneMesh->SetScale( scale );

						//checks for materials used on different parts of the mesh
						unsigned int iPartCount = 0;

						pXmlPoint = pXmlMesh->FirstChildElement( "part" );
						while( pXmlPoint ){
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
						m_oSceneMeshes.push_back( pSceneMesh );
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
					if( sType == "omni" || sType =="spot" )
					{
						float fRadius;
						pXmlElement = pXmlLight->FirstChildElement("Radius");
						if( pXmlElement )
						{

							pXmlElement->QueryFloatAttribute("value",&fRadius);
						}
						if( sType == "spot" )
						{
							SpotLight * pSpot = new SpotLight();
							
							pSpot->SetColor( vec3( r, g, b ) );
							pSpot->SetMultiplier( fMultiplier );
							pSpot->SetRadius( fRadius );
							pSpot->SetPos( vec3( x, y, z ) );
							pSpot->SetRotation( vec3( rX,rY, 0.0 ) );

							pXmlElement = pXmlLight->FirstChildElement("InnerAngle");
							if( pXmlElement )
							{
								float fValue;
								pXmlElement->QueryFloatAttribute("value",&fValue);
								pSpot->SetInnerAngle( fValue );
							}
							pXmlElement = pXmlLight->FirstChildElement("OuterAngle");
							if( pXmlElement )
							{
								float fValue;
								pXmlElement->QueryFloatAttribute("value",&fValue);
								pSpot->SetOuterAngle( fValue );
							}
							pSpot->ComputeBoundingVolume();
							m_oSpotLights.push_back( pSpot );
						}
						else
						{
							OmniLight * pOmni = new OmniLight();
							pOmni->SetColor( vec3( r, g, b ) );
							pOmni->SetMultiplier( fMultiplier );
							pOmni->SetRadius( fRadius );
							pOmni->SetPos( vec3( x, y, z ) );
							m_oOmniLights.push_back( pOmni );
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
