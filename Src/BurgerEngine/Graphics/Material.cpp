#include "Material.h"
#include "BurgerEngine/Graphics/ShaderManager.h"
#include "BurgerEngine/Graphics/TextureManager.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/MaterialManager.h"

#include "BurgerEngine/Graphics/AbstractTexture.h"
#include "BurgerEngine/Graphics/Texture2D.h"
#include "BurgerEngine/Graphics/TextureCubeMap.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include "BurgerEngine/External/Math/Vector.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Material::Material()
{
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
Material::MaterialStatus Material::Initialize( const char * sName )
{
	return _LoadMaterialXML( sName );
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
Material::MaterialStatus Material::_LoadMaterialXML( const char * sName )
{
	TiXmlDocument * pDocument = new TiXmlDocument( sName );

	if(!pDocument->LoadFile())
	{
		std::cerr << "[ReadXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
		return E_LOADING_ERROR;
	}

	TiXmlElement * pRoot = pDocument->FirstChildElement( "material" );

	if( !pRoot )
	{
		return E_LOADING_ERROR;
	}
	//loads a technique
	TiXmlElement * pXmlTechnique = pRoot->FirstChildElement( "technique" );
	while( pXmlTechnique )
	{
		EffectTechnique* pTechnique = new EffectTechnique();
		std::string sTechniqueName = pXmlTechnique->Attribute("name");

		//loads shader program
		Shader* pShader = NULL;
		TiXmlElement * pXmlName = pXmlTechnique->FirstChildElement( "shader" );
		if( !pXmlName )
		{
			return E_LOADING_ERROR;
		}

		pShader = ShaderManager::GrabInstance().AddShader( pXmlName->GetText() );

		if( !pShader )
		{
			return E_SHADER_ERROR;
		}

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
						AbstractTexture* pTexture = TextureManager::GrabInstance().AddTexture( pXmlFileName->GetText() );//_LoadTextureXML( pXmlFileName->GetText() );
						pTechnique->AddUniformTexture( iUnit, pTexture );
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
			TiXmlElement * pXmlVec4 = pXmlUniforms->FirstChildElement( "vec4" );
			while ( pXmlVec4 )
			{
				float x,y,z,w;
				pXmlVec4->QueryFloatAttribute("x",&x);
				pXmlVec4->QueryFloatAttribute("y",&y);
				pXmlVec4->QueryFloatAttribute("z",&z);
				pXmlVec4->QueryFloatAttribute("w",&w);

				int iUniformLocation = glGetUniformLocation( pShader->getHandle(), pXmlVec4->GetText() );
				pTechnique->AddUniformVec4( iUniformLocation, vec4(x,y,z,x) );
				pXmlVec4 = pXmlVec4->NextSiblingElement( "vec4" );
			}
		}

		pShader->QueryStdUniforms();
		pShader->Deactivate();
		pTechnique->SetShader( pShader );

		m_oTechniques[ MaterialManager::GrabInstance().GetTechniqueID( sTechniqueName ) ] = pTechnique;

		pXmlTechnique = pXmlTechnique->NextSiblingElement( "technique" );
	}
	return E_LOADED;
}

bool Material::IsOpaque()
{
	return m_oTechniques.find( EffectTechnique::E_RENDER_OPAQUE ) != m_oTechniques.end();
}

bool Material::IsTransparent()
{
	return m_oTechniques.find( EffectTechnique::E_RENDER_TRANSPARENCY ) != m_oTechniques.end();
}



