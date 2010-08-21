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

#define  M_PI 3.1415926535897932384626433832795

#include "BurgerEngine/External/Osg/Vec3"
#include "BurgerEngine/Graphics/SceneObject.h"

class SceneLight
	: public SceneObject
{
public:
	SceneLight();
	~SceneLight();

public:
	inline void SetInnerAngle( float fValue ){ fInnerAngle = fValue; };
	inline void SetOuterAngle( float fValue){ fCosOut = cos( fValue * M_PI / 180.0); };

	inline void SetSpecular( osg::Vec3 vValue ){ vSpecularColor = vValue; };
	inline void SetAmbient( osg::Vec3 vValue ){ vAmbientColor = vValue; };
	inline void SetDiffuse( osg::Vec3 vValue ){ vDiffuseColor = vValue; };

	inline void SetConstantAtt( float fValue ){ fConstantAttenuation = fValue; };
	inline void SetQuadraticAtt( float fValue ){ fQuadraticAttenuation = fValue; };		
	inline void SetLinearAtt( float fValue ){ fLinearAttenuation = fValue; };

private:

	float		fInnerAngle;
	float		fCosOut;
	
	float		fConstantAttenuation;
	float		fQuadraticAttenuation;
	float		fLinearAttenuation;

	osg::Vec3	vSpecularColor;
	osg::Vec3	vAmbientColor;
	osg::Vec3	vDiffuseColor;
};

#endif //__SCENELIGHT_H__