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

class Shader
{
	public:
	
		Shader(const std::string& sName);
		~Shader() {};

		void Activate();
		void Desactivate();

		///Load/Compile and link the shader;
		bool loadAndCompile(const std::string& sVert, const std::string& sFrag);

		//To communicate with the shader, but we should use some math class
		void setUniformf(const std::string& sName, float fValue);
		void setUniformf( int iUniformLocation, float fValue);

		void setUniformi(const std::string& sName, int fValue);

		void setUniformTexture(const std::string& sName, int iUnit);
		void setUniformMatrix4fv(const std::string& sName, float * pValue);
		
		//Debug Function, should not be used
		GLhandleARB		getHandle();		
	private:
		GLhandleARB		m_oProgram;
		std::string		m_sName;
		bool			m_bIsReady;

};


#endif //__SHADER_H__