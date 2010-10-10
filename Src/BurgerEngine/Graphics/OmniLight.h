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

#ifndef __OMNILIGHT_H__
#define __OMNILIGHT_H__

#include "BurgerEngine/Base/CommonBase.h"

#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/External/Math/Vector.h"

class OmniLight
	: public SceneLight
{
public:

	struct OmniLightQuad
	{
		vec2	vScreenSpaceQuadCenter;
		float	fHalfWidth;
		vec3	vViewSpaceLightPos;
		vec3	vColor;
		float	fInverseRadius;
		float	fMultiplier;
	};

	struct OmniLightVertex
	{
		vec2	vScreenSpaceVertexPos;
		vec3	vColor;
		float	fInverseRadius;
		vec3	vViewSpaceLightPos;
		float	fMultiplier;
	};

	OmniLight();
	~OmniLight();

public:
	inline void SetRadius( float fValue ){ fRadius = fValue; };
	inline float GetRadius(){ return fRadius; };


private:
	float		fRadius;
};

#endif //__OMNILIGHT_H__