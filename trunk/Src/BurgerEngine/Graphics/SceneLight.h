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

	struct SceneLightQuad
	{
		vec3	vViewSpaceLightPos;
		vec3	vColor;
		float	fMultiplier;
	};

	SceneLight();
	~SceneLight();

public:
	inline void SetColor( vec3 vValue ){ vColor = vValue; };
	inline vec3 GetColor(){ return vColor; };

	inline void SetMultiplier( float fValue ){ fMultiplier = fValue; };
	inline float GetMultiplier(){ return fMultiplier; };

private:
	float		fMultiplier;
	vec3		vColor;
};

#endif //__SCENELIGHT_H__