#include "BurgerEngine/Core/StageMain3DScene.h"
#include "BurgerEngine/Input/EventManager.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine\External\TinyXml\TinyXml.h"
#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/Material.h"

#include <iostream>

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StageMain3DScene::StageMain3DScene(std::string const& a_sId)
	: AbstractStage(a_sId)
{
	LoadSceneXML( "../Data/Scenes/testScene.xml" );

	m_pLightZeros = new GLfloat[4];
	
	m_pLightZeros[0] = 0.0;
	m_pLightZeros[1] = 0.0;
	m_pLightZeros[2] = 0.0;
	m_pLightZeros[3] = 1.0;

	m_pLightDir	= new GLfloat[3];
	m_pLightDir[0] = 0.0;
	m_pLightDir[1] = 0.0;
	m_pLightDir[2] = -1.0;

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageMain3DScene::Init()
{
	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageMain3DScene::OnKeyDown));

	Engine::GrabInstance().GrabEventManager().RegisterCallbackKeyboardUpKey(
		EventManager::CallbackKeyboard(this,&StageMain3DScene::OnKeyUp));

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
StageMain3DScene::~StageMain3DScene()
{
	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardDownKey(
		EventManager::CallbackKeyboard(this,&StageMain3DScene::OnKeyDown));

	Engine::GrabInstance().GrabEventManager().UnRegisterCallbackKeyboardUpKey(
		EventManager::CallbackKeyboard(this,&StageMain3DScene::OnKeyUp));

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

	if( m_pLightZeros )
	{
		delete [] m_pLightZeros;
		m_pLightZeros = NULL;
	}
	if( m_pLightDir )
	{
	delete [] m_pLightDir;
	}


}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

void StageMain3DScene::LoadSceneXML( const char * sName )
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

						pSceneMesh->SetPos( osg::Vec4( x, y, z, 1.0 ) );
						pSceneMesh->SetRotation( osg::Vec3( rX, rY, rZ ));
						pSceneMesh->SetScale( scale );

						//checks for materials used on different part of the mesh
						unsigned int iPartCount = 0;

						pXmlPoint = pXmlMesh->FirstChildElement( "part" );
						while( pXmlPoint ){
							++iPartCount;
							TiXmlElement * pXmlMaterial = pXmlPoint->FirstChildElement( "material" );
							if( pXmlMaterial )
							{
								Material * pMaterial = new Material( pXmlMaterial->GetText() );
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
					pLight->SetPos( osg::Vec4( x, y, z, w ) );
					pLight->SetRotation( osg::Vec3( rX, rY, rZ ));

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

						float fCosOut = cos( fValue * M_PI / 180.0);
						//bouh la vieille feinte!
						glLightf( GL_LIGHT0 + iLightCount, GL_QUADRATIC_ATTENUATION, fCosOut );
					}
					pXmlElement = pXmlLight->FirstChildElement("Radius");
					if( pXmlElement )
					{
						float fValue;
						pXmlElement->QueryFloatAttribute("value",&fValue);
						fValue = 1.0 / fValue;
						pLight->SetConstantAtt( fValue );
						glLightf( GL_LIGHT0 + iLightCount, GL_CONSTANT_ATTENUATION, fValue );
					}
					pXmlElement = pXmlLight->FirstChildElement("Color");
					if( pXmlElement )
					{
						float r, g, b;
						pXmlElement->QueryFloatAttribute("r",&r);
						pXmlElement->QueryFloatAttribute("g",&g);
						pXmlElement->QueryFloatAttribute("b",&b);
						pLight->SetSpecular( osg::Vec3( r, g, b ) );

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

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageMain3DScene::OnKeyDown(unsigned char a_cKey)
{
	
	AbstractCamera::CameraFlagEnum e = AbstractCamera::E_CAMERA_LEFT;
	
	switch ( a_cKey )
	{
		case 35 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_LEFT, true );
		break;
		case 36 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_RIGHT, true );
		break;
		case 37 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_FORWARD, true );
		break;
		case 38 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_BACKWARD, true );
		break;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool StageMain3DScene::OnKeyUp(unsigned char a_cKey)
{
	switch ( a_cKey )
	{
		case 35 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_LEFT, false );
		break;
		case 36 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_RIGHT, false );
		break;
		case 37 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_FORWARD, false );
		break;
		case 38 :
			Engine::GetInstance().GetCurrentCamera().SetFlag( AbstractCamera::E_CAMERA_BACKWARD, false );
		break;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void StageMain3DScene::_Render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Engine const& rEngine = Engine::GetInstance();
	AbstractCamera & rCamera = rEngine.GetCurrentCamera();

	gluLookAt(rCamera.GetPos().x(), rCamera.GetPos().y(), rCamera.GetPos().z(),
		rCamera.GetAim().x(), rCamera.GetAim().y(), rCamera.GetAim().z(),
		rCamera.GetUp().x(), rCamera.GetUp().y(), rCamera.GetUp().z());

	rCamera.Update();

	glDisable(GL_BLEND);
	unsigned int i = 0;
	std::vector< SceneLight* >::iterator oLightIt = m_oSceneLights.begin();
	while( oLightIt != m_oSceneLights.end() )
	{
		SceneLight * pLight = (*oLightIt);
		osg::Vec4 oPos = pLight->GetPos();
		osg::Vec3 oRotation = pLight->GetRotation();
		glPushMatrix();
			glTranslatef( oPos.x(), oPos.y() , oPos.z() );
			glRotatef( oRotation.x(), 1, 0, 0 );
			glRotatef( oRotation.y(), 0, 1, 0 );
			glRotatef( oRotation.z(), 0, 0, 1 );
			if( oPos.w() == 1.0 )
			{
				glLightfv( GL_LIGHT0+i, GL_POSITION, m_pLightZeros );
			}
			else
			{
				GLfloat fPos[] = { 1.0, 0.0 , 0.0, 0.0 };
				glLightfv( GL_LIGHT0+i, GL_POSITION, fPos );
			}

			glLightfv( GL_LIGHT0+i, GL_SPOT_DIRECTION, m_pLightDir );
		glPopMatrix();
		++i;
		++oLightIt;
	}

	std::vector< SceneMesh* >::iterator oMeshIt = m_oSceneMeshes.begin();
	while( oMeshIt != m_oSceneMeshes.end() )
	{
		(*oMeshIt)->Draw();
		++oMeshIt;
	}

}