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

#ifndef __SCENELIGHT_H__
#define __SCENELIGHT_H__

#include "BurgerEngine/Base/CommonBase.h"

#include "BurgerEngine/Graphics/SceneObject.h"
#include "BurgerEngine/External/Math/Vector.h"

class SceneLight
	: public SceneObject
{
public:

	struct OmniLightQuad
	{
		vec2	vScreenSpaceCenter;
		float	fHalfWidth;
		vec3	vViewSpacePosition;
		vec3	vColor;
		float	fInverseRadius;
		float	fMultiplier;
	};

	SceneLight();
	~SceneLight();

public:
	inline void SetInnerAngle( float fValue ){ fInnerAngle = fValue; };
	inline void SetOuterAngle( float fValue){ fCosOut = cosf( fValue * (float)M_PI / 180.0f ); };

	inline void SetColor( vec3 vValue ){ vColor = vValue; };
	inline vec3 GetColor(){ return vColor; };

	inline void SetRadius( float fValue ){ fRadius = fValue; };
	inline float GetRadius(){ return fRadius; };

	inline void SetMultiplier( float fValue ){ fMultiplier = fValue; };
	inline float GeMultiplier(){ return fMultiplier; };

private:

	float		fInnerAngle;
	float		fCosOut;
	float		fRadius;

	float		fMultiplier;
	
	vec3	vColor;
};

#endif //__SCENELIGHT_H__