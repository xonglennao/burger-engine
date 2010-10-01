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
	SceneLight();
	~SceneLight();

public:
	inline void SetInnerAngle( float fValue ){ fInnerAngle = fValue; };
	inline void SetOuterAngle( float fValue){ fCosOut = cosf( fValue * (float)M_PI / 180.0f ); };

	inline void SetDiffuse( vec3 vValue ){ vDiffuseColor = vValue; };
	
	inline void SetRadius( float fValue ){ fRadius = fValue; };
	inline float GetRadius(){ return fRadius; };

private:

	float		fInnerAngle;
	float		fCosOut;
	float		fRadius;
	
	vec3	vDiffuseColor;
};

#endif //__SCENELIGHT_H__