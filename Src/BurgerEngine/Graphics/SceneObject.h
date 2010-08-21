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

#ifndef __SCENEOBJECT_H__
#define __SCENEOBJECT_H__

#include <vector>
#include "BurgerEngine/External/Osg/Vec3"
#include "BurgerEngine/External/Osg/Vec4"

class SceneObject
{
public:

	SceneObject();
	~SceneObject();

public:
	inline void SetPos( osg::Vec4 vValue ){ vPosition = vValue; };
	inline void SetPosX ( float fValue ) { vPosition.x() = fValue; };
	inline void SetPosY ( float fValue ) { vPosition.y() = fValue; };
	inline void SetPosZ ( float fValue ) { vPosition.z() = fValue; };
	inline void SetPosW ( float fValue ) { vPosition.w() = fValue; };
	
	inline void SetRotation( osg::Vec3 vValue ){ vRotation = vValue; };
	inline void SetRX ( float fValue ) { vRotation.x() = fValue; };
	inline void SetRY ( float fValue ) { vRotation.y() = fValue; };
	inline void SetRZ ( float fValue ) { vRotation.z() = fValue; };

	inline const osg::Vec4 GetPos() const { return vPosition; };
	inline const osg::Vec3 GetRotation() const { return vRotation; }

protected:
	osg::Vec4	vPosition;
	osg::Vec3	vRotation;
};

#endif //__SCENEOBJECT_H__