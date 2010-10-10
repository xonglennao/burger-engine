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
	: public OmniLightQuad
	{
		vec3 vViewSpaceLightDir;
		vec2 vCosInAndOut;
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
	inline void SetInnerAngle( float fValue ){ fCosIn = cosf( fValue * (float)M_PI / 180.0f ); };
	inline void SetOuterAngle( float fValue){ fCosOut = cosf( fValue * (float)M_PI / 180.0f ); };

	inline float GetCosInnerAngle(){ return fCosIn; };
	inline float GetCosOuterAngle(){ return fCosOut; };

private:

	float		fCosIn;
	float		fCosOut;
};

#endif //__SPOTLIGHT_H__