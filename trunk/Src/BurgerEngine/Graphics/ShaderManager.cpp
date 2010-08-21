#include "shaderManager.h"
#include "shader.h"

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

Shader*	ShaderManager::getShader(const std::string& name)
{
	std::map<std::string,Shader*>::iterator iter = m_mShaders.find(name);
	if (iter == m_mShaders.end())
	{
		std::cerr << "WARNING : cannot Find shader :  " << name << std::endl;
		return NULL;
	}
	return (*iter).second;
}

Shader * ShaderManager::addShader(const std::string& name,const std::string& sVert,const std::string& sFrag)
{
	std::map<std::string,Shader*>::iterator iter = m_mShaders.find(name);
	if (iter == m_mShaders.end())
	{
		Shader* pShader	 =	new Shader(name);
		std::cout << "LOADING SHADER : "<<sFrag<<" - "<<sVert<< std::endl;
		if (pShader->loadAndCompile(sVert,sFrag))
		{
			m_mShaders[name] =	pShader;
			return pShader;
		}
		else
			return NULL;
	}
	else
	{
		std::cerr << "INFO : " << name << " already loadded "<<std::endl;
		return m_mShaders[name];
	}
}