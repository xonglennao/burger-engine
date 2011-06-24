#include "BurgerEngine/Core/RenderComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/CompositeComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/MaterialManager.h"
#include "BurgerEngine/Graphics/RenderingContext.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
RenderComponent::RenderComponent(CompositeComponent* a_pParent):
	AbstractComponent(RENDER, a_pParent),
	m_pMesh(NULL)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
RenderComponent::RenderComponent(AbstractComponent const& a_rToCopy):
	AbstractComponent(RENDER)
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
RenderComponent::~RenderComponent()
{
	//Maybe will be deleted by someone else
	delete m_pMesh;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderComponent::Initialize(TiXmlElement const& a_rParameters)
{
	//Retrieve position
	//First we should check is there is not an desc offset
	//to the parent node
	float x, y, z, rX, rY, rZ, scale;

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

				SetPos( vec3(x,y,z) );
				SetRotation( vec3( rX, rY, rZ ) );
				SetScale( scale );

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
		
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderComponent::_Update( float fFrameTime, float fElapsedTime )
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderComponent::UpdatePos()
{
	m_pMesh->SetPos( GetPos() );
	m_pMesh->ComputeBoundingBox();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderComponent::UpdateScale()
{
	m_pMesh->SetScale( GetScale() );
	m_pMesh->ComputeBoundingBox();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderComponent::UpdateRotation()
{
	m_pMesh->SetRotation( GetRotation() );
	m_pMesh->ComputeBoundingBox();
}
