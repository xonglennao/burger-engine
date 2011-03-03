#include "BurgerEngine/Core/RenderComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/MaterialManager.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
RenderComponent::RenderComponent():
	m_pMesh(NULL)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
RenderComponent::RenderComponent(AbstractComponent const& a_rToCopy)
{
	//Maybe will be deleted by someone else
	delete m_pMesh;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
RenderComponent::~RenderComponent()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderComponent::Initialize(TiXmlElement const& a_rParameters)
{
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
				SceneMesh * pSceneMesh = new SceneMesh( pMesh );

				/*pSceneMesh->SetPos( vec3( x, y, z ) );
				pSceneMesh->SetRotation( vec3( rX, rY, rZ ));
				pSceneMesh->SetScale( scale );*/

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
							pSceneMesh->AddMaterial( pMaterial );
						}
					}			

					pPart = pPart->NextSiblingElement( "part" );
				}

				pSceneMesh->SetPartCount( iPartCount );

				// Hum how will we do this??
				/*if( pSceneMesh->IsOpaque() )
				{
					m_oSceneMeshes.push_back( pSceneMesh );
				}
				else if( pSceneMesh->IsTransparent() )
				{
					m_oTransparentSceneMeshes.push_back( pSceneMesh );
				}*/
			}

		}
		//Add itsefl to the render context
		//Or maybe juste the mesh should register?? Or To the rendering pipeline( which does not exist so far)
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderComponent::_Update()
{
}