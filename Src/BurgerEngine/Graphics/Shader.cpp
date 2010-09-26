#include "shader.h"
#include "shaderTool.h"
#include <iostream>

Shader::Shader(const std::string& sName):
m_sName(sName)
{
	m_bIsReady = false;
}

bool Shader::loadAndCompile(const std::string& sVert, const std::string& sFrag)
{
	GLhandleARB so[2]; 
	memset(so, 0, sizeof(GLhandleARB)*2);

	///Loading the shader
	so[0] = loadShader(sVert.c_str()); 
	if(so[0]==0){
		std::cerr << "loading shader "+sVert+" failed" << std::endl;
		return false;
	}

	///Compiling the shader, done by the graphic card
	if(!compileShader(so[0])){ 
		std::cerr << "compiling shader "+sVert+" failed" << std::endl;
		return false;
	}

	so[1] = loadShader(sFrag.c_str());
	if(so[1]==0){
		std::cerr << "loading shader "+sFrag+" failed " << std::endl;
		return false;
	}
	if(!compileShader(so[1])){
		std::cerr << "compiling shader "+sFrag+" failed " << std::endl;
		return false;
	}

	m_oProgram = linkShaders(so,2);
	m_bIsReady = true;
	return true;
}

void Shader::Activate()
{
	if (m_bIsReady)
		glUseProgramObjectARB(m_oProgram);
}

void Shader::Desactivate()
{
	glUseProgramObjectARB(0);
}

void Shader::setUniformf(const std::string& sName, float fValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform1f(glGetUniformLocation(m_oProgram, sName.c_str()), fValue);
}

void Shader::setUniformf( int iUniformLocation, float fValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform1f( iUniformLocation, fValue);
}

void Shader::setUniformi(const std::string& sName, int fValue)
{
	/// Need to Activate the shader before, but we are not doing it
	/// in here to save computation time (if we are setting several variable at the same time)
	glUniform1i(glGetUniformLocation(m_oProgram, sName.c_str()), fValue);
}

void Shader::setUniformTexture(const std::string& sName, int iUnit)
{
	glUniform1iARB(glGetUniformLocationARB(m_oProgram, sName.c_str()), iUnit);
}

void Shader::setUniformMatrix4fv(const std::string& sName, float * pValue)
{
	glUniformMatrix4fvARB(glGetUniformLocationARB(m_oProgram,sName.c_str()),1,GL_FALSE,pValue);
}


GLhandleARB	Shader::getHandle()
{
	return m_oProgram;
}	