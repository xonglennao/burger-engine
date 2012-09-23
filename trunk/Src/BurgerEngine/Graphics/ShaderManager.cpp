#include "shaderManager.h"
#include "shader.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

#include <iostream>

ShaderManager::ShaderManager()
{
}

void ShaderManager::clear()
{
	for (std::map<std::string,Shader*>::iterator iter = m_mShaders.begin(); iter != m_mShaders.end() ; ++iter)
		delete (*iter).second;
	m_mShaders.clear();
}

Shader*	ShaderManager::GetShader(const std::string& name)
{
	std::map<std::string,Shader*>::iterator iter = m_mShaders.find(name);
	if (iter == m_mShaders.end())
	{
		std::cerr << "WARNING : cannot Find shader :  " << name << std::endl;
		return NULL;
	}
	return (*iter).second;
}

Shader * ShaderManager::AddShader(const char * sName )
{
	std::map<std::string,Shader*>::iterator iter = m_mShaders.find( sName );
	if ( iter == m_mShaders.end() )
	{
		TiXmlDocument * pDocument = new TiXmlDocument( sName );

		if(!pDocument->LoadFile())
		{
  			std::cerr << "[ReadXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
			return NULL;
		}
	
		TiXmlElement * pRoot = pDocument->FirstChildElement( "shader" );	
		if( pRoot )
		{
			std::string sShaderName, sVertexShader, sPixelShader;
			//loads shader program
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

			Shader* pShader	 =	new Shader();
			std::cout << "LOADING SHADER : "<< sVertexShader <<" - "<< sPixelShader << std::endl << std::endl;
			assert(pShader->LoadAndCompile( sVertexShader, sPixelShader ) );
			return m_mShaders[ sName ] = pShader;
		}
		return NULL;
	}
	return m_mShaders[ sName ];
}