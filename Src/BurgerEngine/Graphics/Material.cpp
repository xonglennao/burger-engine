#include "Material.h"
#include "BurgerEngine/Graphics/ShaderManager.h"
#include "BurgerEngine/Graphics/TextureManager.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

Material::Material( const char * sName )
	: m_pShader( NULL )
{
	LoadMaterialXML( sName );
}

Material::~Material()
{
};

void Material::Activate()
{
	m_pShader->Activate();
	ActivateTextures();
	CommitFloatUniforms();
}

void Material::CommitFloatUniforms()
{
	std::map< std::string, float >::iterator it = m_oUniformFloatsMap.begin();
	while( it != m_oUniformFloatsMap.end() )
	{
		m_pShader->setUniformf( (*it).first, (*it).second );
		++it;
	}

}

void Material::ActivateTextures()
{
	std::map< int, Texture2D* >::iterator it = m_oUniformTextures2DMap.begin();
	while( it != m_oUniformTextures2DMap.end() )
	{
		glActiveTexture( GL_TEXTURE0 + (*it).first );
		(*it).second->Activate();
		++it;
	}
}

void Material::LoadMaterialXML( const char * sName )
{
	TiXmlDocument * pDocument = new TiXmlDocument( sName );

	if(!pDocument->LoadFile())
	{
  		std::cerr << "[ReadXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
	}
	
	TiXmlElement * pRoot = pDocument->FirstChildElement( "material" );

	if( pRoot )
	{
		std::string sName, sVertexShader, sPixelShader;
		
		//loads shader program
		TiXmlElement * pXmlName = pRoot->FirstChildElement( "shadername" );
		if( pXmlName )
		{
			sName = std::string( pXmlName->GetText() );
		}

		TiXmlElement * pXmlVertex = pRoot->FirstChildElement( "vertexshader" );
		if( pXmlVertex )
		{
			sVertexShader = std::string( pXmlVertex->GetText() );
		}

		TiXmlElement * pXmlPixel = pRoot->FirstChildElement( "pixelshader" );
		if( pXmlPixel )
		{
			sPixelShader = std::string( pXmlPixel->GetText() );
		}
		
		m_pShader = ShaderManager::GrabInstance().addShader( sName, sVertexShader, sPixelShader );
		m_pShader->Activate();

		//gets the samplers used by the material
		TextureManager & textureManager = TextureManager::GrabInstance();
		TiXmlElement * pXmlSamplers = pRoot->FirstChildElement( "samplers" );
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
					m_pShader->setUniformTexture( pXmlParam->GetText(), iUnit );
				}
				TiXmlElement * pXmlFileName = pXmlSampler->FirstChildElement( "filename" );
				if( pXmlFileName )
				{
					Texture2D* pTexture2D = textureManager.getTexture2D( pXmlFileName->GetText() );
					m_oUniformTextures2DMap[ iUnit ] = pTexture2D;
				}

				pXmlSampler = pXmlSampler->NextSiblingElement( "sampler" );
			}
		}
		m_pShader->Desactivate();

		//gets uniform float variables
		TiXmlElement * pXmlUniforms = pRoot->FirstChildElement( "uniforms" );
		if( pXmlUniforms )
		{
			TiXmlElement * pXmlFloat = pXmlUniforms->FirstChildElement( "float" );
			while ( pXmlFloat )
			{
				float fValue;
				pXmlFloat->QueryFloatAttribute("value",&fValue);
				m_oUniformFloatsMap[ pXmlFloat->GetText() ] = fValue;
				pXmlFloat = pXmlFloat->NextSiblingElement( "float" );
			}
		}
	}
}



