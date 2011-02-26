#include "Material.h"
#include "BurgerEngine/Graphics/ShaderManager.h"
#include "BurgerEngine/Graphics/TextureManager.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/MaterialManager.h"

#include "BurgerEngine/Graphics/AbstractTexture.h"
#include "BurgerEngine/Graphics/Texture2D.h"
#include "BurgerEngine/Graphics/TextureCubeMap.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Material::Material( const char * sName )
{
	_LoadMaterialXML( sName );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Material::~Material()
{
	std::map< EffectTechnique::RenderingTechnique, EffectTechnique* >::iterator it = m_oTechniques.begin();
	while( it != m_oTechniques.end() )
	{
		delete (*it).second;
		(*it).second = NULL;
		++it;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool Material::Activate( EffectTechnique::RenderingTechnique eTechnique )
{
	std::map< EffectTechnique::RenderingTechnique, EffectTechnique* >::iterator oIt = m_oTechniques.find(eTechnique);
	if (oIt != m_oTechniques.end())
	{
		oIt->second->Activate();
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Material::Deactivate( EffectTechnique::RenderingTechnique eTechnique )
{
	std::map< EffectTechnique::RenderingTechnique, EffectTechnique* >::iterator oIt = m_oTechniques.find(eTechnique);
	if (oIt != m_oTechniques.end())
	{
		oIt->second->Deactivate();
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Material::_LoadMaterialXML( const char * sName )
{
	TiXmlDocument * pDocument = new TiXmlDocument( sName );

	if(!pDocument->LoadFile())
	{
  		std::cerr << "[ReadXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
	}
	
	TiXmlElement * pRoot = pDocument->FirstChildElement( "material" );

	if( pRoot )
	{
		//loads a technique
		TiXmlElement * pXmlTechnique = pRoot->FirstChildElement( "technique" );
		while( pXmlTechnique )
		{
			EffectTechnique* pTechnique = new EffectTechnique();
			std::string sTechniqueName = pXmlTechnique->Attribute("name");
			
			std::string sName, sVertexShader, sPixelShader;
		
			//loads shader program
			TiXmlElement * pXmlName = pXmlTechnique->FirstChildElement( "shadername" );
			if( pXmlName )
			{
				sName = std::string( pXmlName->GetText() );
			}

			TiXmlElement * pXmlVertex = pXmlTechnique->FirstChildElement( "vertexshader" );
			if( pXmlVertex )
			{
				sVertexShader = std::string( pXmlVertex->GetText() );
			}

			TiXmlElement * pXmlPixel = pXmlTechnique->FirstChildElement( "pixelshader" );
			if( pXmlPixel )
			{
				sPixelShader = std::string( pXmlPixel->GetText() );
			}
		
			Shader* pShader = ShaderManager::GrabInstance().addShader( sName, sVertexShader, sPixelShader );
			pShader->Activate();

			//gets the textures used by the material
			TiXmlElement * pXmlTextures = pXmlTechnique->FirstChildElement( "textures" );
			if( pXmlTextures )
			{
				TiXmlElement * pXmlTexture = pXmlTextures->FirstChildElement( "texture" );
				while ( pXmlTexture )
				{
					int iUnit;
					TiXmlElement * pXmlParam = pXmlTexture->FirstChildElement( "uniformname" );
					if( pXmlParam )
					{
						
						pXmlParam->QueryIntAttribute("unit",&iUnit);
						pShader->setUniformTexture( pXmlParam->GetText(), iUnit );

						TiXmlElement * pXmlFileName = pXmlTexture->FirstChildElement( "filename" );
						if( pXmlFileName )
						{
							AbstractTexture* pTexture = _LoadTextureXML( pXmlFileName->GetText() );
							
							if( pTexture )
							{
								pTechnique->AddUniformTexture( iUnit, pTexture );
							}
						}
					}

					pXmlTexture = pXmlTexture->NextSiblingElement( "texture" );
				}
			}

			//gets uniform float variables
			TiXmlElement * pXmlUniforms = pXmlTechnique->FirstChildElement( "uniforms" );
			if( pXmlUniforms )
			{
				TiXmlElement * pXmlFloat = pXmlUniforms->FirstChildElement( "float" );
				while ( pXmlFloat )
				{
					float fValue;
					pXmlFloat->QueryFloatAttribute("value",&fValue);

					int iUniformLocation = glGetUniformLocation( pShader->getHandle(), pXmlFloat->GetText() );
					pTechnique->AddUniformFloat( iUniformLocation, fValue );
					pXmlFloat = pXmlFloat->NextSiblingElement( "float" );
				}
			}
			
			pShader->QueryStdUniforms();
			pShader->Deactivate();
			pTechnique->SetShader( pShader );

			m_oTechniques[ MaterialManager::GrabInstance().GetTechniqueID( sTechniqueName ) ] = pTechnique;

			pXmlTechnique = pXmlTechnique->NextSiblingElement( "technique" );
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractTexture* Material::_LoadTextureXML( const char * sName )
{
	TiXmlDocument * pDocument = new TiXmlDocument( sName );

	if(!pDocument->LoadFile())
	{
  		std::cerr << "[ReadXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
	}
	
	TiXmlElement * pRoot = pDocument->FirstChildElement( "texture" );

	if( pRoot )
	{
		int iUseMipMap = 0, iLinearFiltering = 0, iClampS = 0, iClampT = 0;
		pRoot->QueryIntAttribute( "mipmap", &iUseMipMap );
		pRoot->QueryIntAttribute( "linear", &iLinearFiltering );
		pRoot->QueryIntAttribute( "clamps", &iClampS );
		pRoot->QueryIntAttribute( "clampt", &iClampT );

		std::string sTextureType = pRoot->Attribute("type");
		
		TiXmlElement * pXmlFileName = pRoot->FirstChildElement( "filename" );
		if( pXmlFileName )
		{
			AbstractTexture * pTexture = NULL;
			if( sTextureType == "texture2D" )
			{
				pTexture = TextureManager::GrabInstance().getTexture2D( pXmlFileName->GetText(), iUseMipMap != 0 , iLinearFiltering != 0, iClampS != 0, iClampT != 0 );
			}
			else
			{
				int iClampR;
				pRoot->QueryIntAttribute( "clampr", &iClampR );
				if(sTextureType == "textureCube")
				{
					pTexture = TextureManager::GrabInstance().getTextureCubeMap( pXmlFileName->GetText(), iUseMipMap != 0 , iLinearFiltering != 0, iClampS != 0, iClampT != 0 );
				}
				else if(sTextureType == "texture3D")
				{
					int iWidth, iHeight, iDepth;
					pRoot->QueryIntAttribute( "width", &iWidth );
					pRoot->QueryIntAttribute( "height", &iHeight );
					pRoot->QueryIntAttribute( "depth", &iDepth );
					pTexture = TextureManager::GrabInstance().getTexture3DFrom2DFile( pXmlFileName->GetText(), iUseMipMap != 0 , iLinearFiltering != 0, iClampS != 0, iClampT != 0, iClampR != 0 );
				}
			}
			return pTexture;
		}
	}

	return NULL;
}

bool Material::IsOpaque()
{
	return m_oTechniques.find( EffectTechnique::E_RENDER_OPAQUE ) != m_oTechniques.end();
}

bool Material::IsTransparent()
{
	return m_oTechniques.find( EffectTechnique::E_RENDER_TRANSPARENCY ) != m_oTechniques.end();
}



