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

	enum LightType
	{
		 E_DIRECTIONAL	= 1
		,E_OMNI_LIGHT	= 1 << 1
		,E_SPOT_LIGHT	= 1 << 2 | E_OMNI_LIGHT
		,E_SPOT_SHADOW	= 1 << 3 | E_SPOT_LIGHT
	};

	struct SceneLightQuad
	{
		vec3	vViewSpaceLightPos;
		vec3	vColor;
		float	fMultiplier;
	};

	struct DirectionalLightVertex
	{
		vec2	vScreenSpaceVertexPos;
		vec3	vColor;
		vec3	vViewSpaceLightPos;
		float	fMultiplier;
	};

	SceneLight();
	~SceneLight();

public:
	void SetColor( vec3 vValue ){ vColor = vValue; };
	vec3 GetColor(){ return vColor; };

	void SetMultiplier( float fValue ){ fMultiplier = fValue; };
	float GetMultiplier(){ return fMultiplier; };

	void SetType(LightType a_eType){m_eLightType = a_eType;}
	LightType GetType() const {return m_eLightType;}
private:
	float		fMultiplier;
	LightType	m_eLightType;
	vec3		vColor;
};

#endif //__SCENELIGHT_H__