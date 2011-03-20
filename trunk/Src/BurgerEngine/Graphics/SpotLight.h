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

#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

#include "BurgerEngine/Base/CommonBase.h"

#include "BurgerEngine/Graphics/OmniLight.h"
#include "BurgerEngine/External/Math/Vector.h"

class SpotLight
	: public OmniLight
{
public:

	struct SpotLightQuad
	: public SceneLightQuad
	{
		vec4	vLeftRightTopBottom;
		float	fInverseRadius;
		vec3	vViewSpaceLightDir;
		vec2	vCosInAndOut;
	};

	struct SpotLightVertex
	: public OmniLightVertex
	{
		vec3 vViewSpaceLightDir;
		vec2 vCosInAndOut;
	};

	SpotLight();
	~SpotLight();

public:
	virtual void	ComputeBoundingBox();

	void SetInnerAngle( float fValue ){ m_fCosIn = cosf( fValue * DEG_TO_RAD ); };
	void SetOuterAngle( float fValue){ m_fCosOut = cosf( fValue * DEG_TO_RAD ); };

	float GetCosInnerAngle(){ return m_fCosIn; };
	float GetCosOuterAngle(){ return m_fCosOut; };

	const vec3* GetFarPlanePoints(){ return m_pFarPlanePoints; };

private:

	float		m_fCosIn;
	float		m_fCosOut;

	vec3		m_pFarPlanePoints[4];
};

#endif //__SPOTLIGHT_H__