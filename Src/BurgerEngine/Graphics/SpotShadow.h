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

#ifndef __SPOTSHADOW_H__
#define __SPOTSHADOW_H__

#include "BurgerEngine/Base/CommonBase.h"

#include "BurgerEngine/Graphics/SpotLight.h"
#include "BurgerEngine/External/Math/Vector.h"

class FBO;

class SpotShadow
	: public SpotLight
{
public:

	const static int iShadowMapSize = 512;

	struct SpotShadowQuad
	: public SpotLightQuad
	{
	};

	struct SpotShadowVertex
	: public SpotLightVertex
	{
	};

	SpotShadow();
	~SpotShadow();

	void ActivateBuffer();
	void DesactivateBuffer();
	void ActivateDepthTexture();

	void			SetMatrix( const float4x4& mMatrix){ m_mViewProjMatrix = mMatrix; };
	const float4x4&	GetMatrix(){ return m_mViewProjMatrix; };

private:
	FBO*		m_oDepth;
	float4x4	m_mViewProjMatrix; 
};

#endif //__SPOTSHADOW_H__