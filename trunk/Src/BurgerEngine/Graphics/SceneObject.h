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

	void SetViewZ( float vValue ){ m_fViewZ = vValue; }
	
	void SetRotation( vec3 vValue ){ m_f3Rotation = vValue; UpdateRotationMatrix(); }
	void SetRX ( float fValue ) { m_f3Rotation.x = fValue; UpdateRotationMatrix(); }
	void SetRY ( float fValue ) { m_f3Rotation.y = fValue; UpdateRotationMatrix(); }
	void SetRZ ( float fValue ) { m_f3Rotation.z = fValue; UpdateRotationMatrix(); }

	vec3 const&		GetPos() const { return m_f3Position; }
	float const&	GetViewZ() const { return m_fViewZ; }
	vec3 const&		GetRotation() const { return m_f3Rotation; }

	virtual void	ComputeBoundingBox(){};
	const float*	GetBoundingBox(){ return m_pBoundingBox; };

protected:
	void UpdateRotationMatrix() { m_mRotationMatrix = rotateZ( m_f3Rotation.z * DEG_TO_RAD ) * rotateY( m_f3Rotation.y * DEG_TO_RAD ) * rotateX( m_f3Rotation.x * DEG_TO_RAD ); };

	/// Position
	vec3		m_f3Position;
	
	// view space position on z axis
	float		m_fViewZ;
	
	/// Rotation
	vec3		m_f3Rotation;
		
	float *		m_pBoundingBox;

	float4x4	m_mRotationMatrix;
};

#endif //__SCENEOBJECT_H__