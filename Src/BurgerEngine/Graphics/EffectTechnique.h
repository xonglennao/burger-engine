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

#include "BurgerEngine/Graphics/AbstractTexture.h"
#include "BurgerEngine/Graphics/Texture2D.h"
#include "BurgerEngine/Graphics/TextureCubeMap.h"
#include "BurgerEngine/Graphics/Texture3D.h"

class EffectTechnique
{
public:

	enum RenderingTechnique
	{
		E_RENDER_GBUFFER,
		E_RENDER_OPAQUE,
		E_RENDER_TRANSPARENCY,
		E_RENDER_SHADOW_MAP = 100
	};

	EffectTechnique();
	~EffectTechnique();

	void Activate();
	inline void Deactivate(){ m_pShader->Deactivate(); }

	void AddUniformTexture( int iUnit, AbstractTexture* pTexture ){ m_oUniformTextures2DMap[ iUnit ] = pTexture; };
	//void AddUniformTextureCubeMap( int iUnit, TextureCubeMap* pTextureCube ){ m_oUniformTextures2DMap[ iUnit ] = pTextureCube; };
	void AddUniformFloat( int iUniformLocation, float fValue ){ m_oUniformFloatsMap[ iUniformLocation ] = fValue; };
	void SetShader( Shader* pShader ){ m_pShader = pShader; };

private:
	void ActivateTextures();
	void CommitUniformFloats();

private:
	Shader * m_pShader;
	std::map< int, AbstractTexture* > m_oUniformTextures2DMap;
	std::map< int, float > m_oUniformFloatsMap;
};

#endif //__EFFECT_TECHNIQUE_H__