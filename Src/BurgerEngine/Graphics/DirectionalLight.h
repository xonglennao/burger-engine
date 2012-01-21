/*************************************
*
*		BurgerEngine Project
*		
*		Created :	13/11/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __DIRECTIONALLIGHT_H__
#define __DIRECTIONALLIGHT_H__

#include "BurgerEngine/Base/CommonBase.h"

#include "BurgerEngine/Graphics/SceneLight.h"

class FBO;

class DirectionalLight
	: public SceneLight
{
public:
	
	const static int iShadowMapSize = 512;
	static float pCascadeSizes[4];
	const static int iCascadeCount = 4;

	DirectionalLight();
	~DirectionalLight();

	void ActivateBuffer();
	void DeactivateBuffer();
	void ActivateDepthTexture();

	void			SetMatrix( const float4x4& mMatrix, unsigned int iIndex );
	const float4x4&	GetMatrix( unsigned int iIndex ) const;
	const float4x4*	GetMatrices() const;

private:
	FBO*		m_oDepth;
	float4x4*	m_pViewProjMatrix; 
};

#endif //__DIRECTIONALLIGHT_H__