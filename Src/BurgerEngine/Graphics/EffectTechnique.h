/*************************************
*
*		BurgerEngine Project
*		
*		Created :	26/09/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __EFFECT_TECHNIQUE_H__
#define __EFFECT_TECHNIQUE_H__


#include <iostream>
#include <string>
#include <map>

#include "BurgerEngine/Graphics/Shader.h"

class Texture2D;
class EffectTechnique
{
public:

	enum RenderingTechnique
	{
		E_RENDER_GBUFFER,
		E_RENDER_OPAQUE,
		E_RENDER_SHADOW_MAP = 100
	};

	EffectTechnique();
	~EffectTechnique();

	void Activate();
	inline void Desactivate(){ m_pShader->Desactivate(); }

	void AddUniformTexture( int iUnit, Texture2D* pTexture2D ){ m_oUniformTextures2DMap[ iUnit ] = pTexture2D; };
	void AddUniformFloat( int iUniformLocation, float fValue ){ m_oUniformFloatsMap[ iUniformLocation ] = fValue; };
	void SetShader( Shader* pShader ){ m_pShader = pShader; };

private:
	void ActivateTextures();
	void CommitUniformFloats();

private:
	Shader * m_pShader;
	std::map< int, Texture2D* > m_oUniformTextures2DMap;
	std::map< int, float > m_oUniformFloatsMap;
};

#endif //__EFFECT_TECHNIQUE_H__