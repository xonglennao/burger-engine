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

#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

#include "CommonGraphics.h"
#include <map>

class Shader
{
	public:
	
		enum StandardUniforms
		{
			E_STD_INV_VIEWPORT
		};

		Shader(const std::string& sName);
		~Shader() {};

		void Activate();
		void Desactivate();

		///Load/Compile and link the shader;
		bool loadAndCompile(const std::string& sVert, const std::string& sFrag);

		//To communicate with the shader, but we should use some math class
		void setUniformf(const std::string& sName, float fValue);
		void setUniformf( int iUniformLocation, float fValue);
		void setUniform2fv( int iUniformLocation, int iCount, float * pValue);

		void setUniformi(const std::string& sName, int fValue);
		void setUniformi( int iUniformLocation, int fValue);

		void setUniformTexture(const std::string& sName, int iUnit);
		void setUniformTexture( int iUniformLocation, int iUnit);

		void setUniformMatrix4fv(const std::string& sName, float * pValue);
		void setUniformMatrix4fv( int iUniformLocation, float * pValue);		
		//Debug Function, should not be used
		GLhandleARB		getHandle();	

		void QueryStdUniforms();
		void CommitStdUniforms();

private:
		GLhandleARB		m_oProgram;
		std::string		m_sName;
		bool			m_bIsReady;

		std::map< StandardUniforms, int > m_oStdUniformsMap;
};


#endif //__SHADER_H__