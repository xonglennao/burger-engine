/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/08/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __SHADERMANAGER_H__
#define __SHADERMANAGER_H__

#include <string>
#include <map>

#include "BurgerEngine/Base/Singleton.h"
#include "shaderTool.h"

///Forward declaration
class Shader;

class ShaderManager:public Singleton<ShaderManager>
{
	friend class Singleton<ShaderManager>;

	public:
		Shader* GetShader(const std::string& name);
		Shader* AddShader(const char* sName );

		void clear();
		inline bool isSupported(const std::string& ext){
				return isExtensionSupported((char*)ext.c_str());};

	protected:
		std::map<std::string,Shader*> m_mShaders;

		ShaderManager();
		virtual ~ShaderManager(){clear();};
};

#endif //__SHADERMANAGER_H__