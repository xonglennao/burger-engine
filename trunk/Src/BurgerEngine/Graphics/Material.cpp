#include "Material.h"
#include "BurgerEngine/Graphics/ShaderManager.h"
#include "BurgerEngine/Graphics/TextureManager.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/MaterialManager.h"

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
void Material::Desactivate( EffectTechnique::RenderingTechnique eTechnique )
{
	std::map< EffectTechnique::RenderingTechnique, EffectTechnique* >::iterator oIt = m_oTechniques.find(eTechnique);
	if (oIt != m_oTechniques.end())
	{
		oIt->second->Desactivate();
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

			//gets the samplers used by the material
			TextureManager & textureManager = TextureManager::GrabInstance();
			TiXmlElement * pXmlSamplers = pXmlTechnique->FirstChildElement( "samplers" );
			if( pXmlSamplers )
			{
				TiXmlElement * pXmlSampler = pXmlSamplers->FirstChildElement( "sampler" );
				while ( pXmlSampler )
				{
					int iUnit;
					TiXmlElement * pXmlParam = pXmlSampler->FirstChildElement( "uniformname" );
					if( pXmlParam )
					{
						pXmlParam->QueryIntAttribute("unit",&iUnit);
						pShader->setUniformTexture( pXmlParam->GetText(), iUnit );
					}
					TiXmlElement * pXmlFileName = pXmlSampler->FirstChildElement( "filename" );
					if( pXmlFileName )
					{
						Texture2D* pTexture2D = textureManager.getTexture2D( pXmlFileName->GetText() );
						pTechnique->AddUniformTexture( iUnit, pTexture2D );

					}

					pXmlSampler = pXmlSampler->NextSiblingElement( "sampler" );
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
			pShader->Desactivate();
			pTechnique->SetShader( pShader );

			m_oTechniques[ MaterialManager::GrabInstance().GetTechniqueID( sTechniqueName ) ] = pTechnique;

			pXmlTechnique = pXmlTechnique->NextSiblingElement( "technique" );
		}
	}
}

bool Material::IsOpaque()
{
	return m_oTechniques.find( EffectTechnique::E_RENDER_OPAQUE ) != m_oTechniques.end();
}



