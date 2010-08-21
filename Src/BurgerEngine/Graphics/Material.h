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

#ifndef __MATERIAL_H__
#define __MATERIAL_H__


#include <iostream>
#include <string>
#include <map>

#include "BurgerEngine/Graphics/shader.h"
#include "BurgerEngine/Graphics/Texture2D.h"

class Shader;

class Material
{
public:

	Material( const char * sName );
	~Material();

	void Activate();
	inline void Desactivate(){ m_pShader->Desactivate(); }



private:
	void ActivateTextures();
	void CommitFloatUniforms();
	void LoadMaterialXML( const char * sName );

private:
	Shader * m_pShader;
	std::map< int, Texture2D* > m_oUniformTextures2DMap;
	std::map< std::string, float > m_oUniformFloatsMap;

};

#endif //__MATERIAL_H__