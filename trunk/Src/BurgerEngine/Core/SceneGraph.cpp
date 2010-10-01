#include "BurgerEngine/Core/SceneGraph.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"

#include "BurgerEngine/Input/EventManager.h"

#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/MaterialManager.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include <iostream>

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SceneGraph::SceneGraph()
{
	LoadSceneXML( "../Data/Scenes/testScene.xml" );
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

	std::vector< SceneLight* >::iterator oLightIt = m_oSceneLights.begin();
	while( oLightIt != m_oSceneLights.end() )
	{
		delete (*oLightIt);
		(*oLightIt) = NULL;
		++oLightIt;
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
		unsigned int iLightCount = 0;
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

						pSceneMesh->SetPos( vec4( x, y, z, 1.0 ) );
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
					float w;
					pXmlObject->QueryFloatAttribute("w",&w);

					SceneLight * pLight = new SceneLight;					
					pLight->SetPos( vec4( x, y, z, w ) );
					pLight->SetRotation( vec3( rX, rY, rZ ));

					TiXmlElement * pXmlElement;
					
					pXmlElement = pXmlLight->FirstChildElement("InnerAngle");
					if( pXmlElement )
					{
						float fValue;
						pXmlElement->QueryFloatAttribute("value",&fValue);
						pLight->SetInnerAngle( fValue );
						glLightf( GL_LIGHT0 + iLightCount , GL_SPOT_CUTOFF, fValue );
					}
					pXmlElement = pXmlLight->FirstChildElement("OuterAngle");
					if( pXmlElement )
					{
						float fValue;
						pXmlElement->QueryFloatAttribute("value",&fValue);
						pLight->SetOuterAngle( fValue );

						float fCosOut = cosf( fValue * (float)M_PI / 180.0f );
						//bouh la vieille feinte!
						glLightf( GL_LIGHT0 + iLightCount, GL_QUADRATIC_ATTENUATION, fCosOut );
					}
					pXmlElement = pXmlLight->FirstChildElement("Radius");
					if( pXmlElement )
					{
						float fValue;
						pXmlElement->QueryFloatAttribute("value",&fValue);
						pLight->SetRadius( fValue );
						fValue = 1.0f / fValue;
						glLightf( GL_LIGHT0 + iLightCount, GL_CONSTANT_ATTENUATION, fValue );
					}
					pXmlElement = pXmlLight->FirstChildElement("Color");
					if( pXmlElement )
					{
						float r, g, b;
						pXmlElement->QueryFloatAttribute("r",&r);
						pXmlElement->QueryFloatAttribute("g",&g);
						pXmlElement->QueryFloatAttribute("b",&b);

						GLfloat pColor[] = { r, g, b };
						glLightfv( GL_LIGHT0 + iLightCount , GL_AMBIENT, pColor );
						glLightfv( GL_LIGHT0 + iLightCount , GL_SPECULAR, pColor );
						glLightfv( GL_LIGHT0 + iLightCount , GL_DIFFUSE, pColor );
					}

					m_oSceneLights.push_back( pLight );
					++iLightCount;
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
