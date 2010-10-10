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
#include "BurgerEngine/External/Math/Vector.h"


/// \name	SceneObject.h
/// \brief	The main object in a scene, can be positioned, rotated etc...
class SceneObject
{
public:

	/// \brief Constructor
	SceneObject();

	/// \brief Destructor
	~SceneObject();

public:
	void SetPos( vec3 vValue ){ m_f3Position = vValue; }
	void SetPosX ( float fValue ) { m_f3Position.x = fValue; }
	void SetPosY ( float fValue ) { m_f3Position.y = fValue; }
	void SetPosZ ( float fValue ) { m_f3Position.z = fValue; }
	
	void SetRotation( vec3 vValue ){ m_f3Rotation = vValue; }
	void SetRX ( float fValue ) { m_f3Rotation.x = fValue; }
	void SetRY ( float fValue ) { m_f3Rotation.y = fValue; }
	void SetRZ ( float fValue ) { m_f3Rotation.z = fValue; }

	vec3 const&  GetPos() const { return m_f3Position; }
	vec3 const&  GetRotation() const { return m_f3Rotation; }

protected:

	/// Position
	vec3	m_f3Position;

	/// Rotation
	vec3	m_f3Rotation;
};

#endif //__SCENEOBJECT_H__