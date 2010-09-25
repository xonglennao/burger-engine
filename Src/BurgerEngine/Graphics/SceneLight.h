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
	inline void SetOuterAngle( float fValue){ fCosOut = cosf( fValue * M_PI / 180.0f ); };

	inline void SetSpecular( vec3 vValue ){ vSpecularColor = vValue; };
	inline void SetAmbient( vec3 vValue ){ vAmbientColor = vValue; };
	inline void SetDiffuse( vec3 vValue ){ vDiffuseColor = vValue; };

	inline void SetConstantAtt( float fValue ){ fConstantAttenuation = fValue; };
	inline void SetQuadraticAtt( float fValue ){ fQuadraticAttenuation = fValue; };		
	inline void SetLinearAtt( float fValue ){ fLinearAttenuation = fValue; };

private:

	float		fInnerAngle;
	float		fCosOut;
	
	float		fConstantAttenuation;
	float		fQuadraticAttenuation;
	float		fLinearAttenuation;

	vec3	vSpecularColor;
	vec3	vAmbientColor;
	vec3	vDiffuseColor;
};

#endif //__SCENELIGHT_H__