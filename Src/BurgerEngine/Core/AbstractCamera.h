/*************************************
*
*		BurgerEngine Project
*		
*		Created :	16/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __ABSTRACTCAMERA_H__
#define __ABSTRACTCAMERA_H__

#include "BurgerEngine/External/Math/Vector.h"

/// \class AbstractCamera
/// \brief Interface class for every camera
class AbstractCamera
{
public:
	enum CameraFlagEnum
	{
		 E_CAMERA_FORWARD		=	1
		,E_CAMERA_BACKWARD		=	1 << 1 
		,E_CAMERA_LEFT			=	1 << 2
		,E_CAMERA_RIGHT			=	1 << 3
		,E_DOF_NEAR_FORWARD		=	1 << 4
		,E_DOF_NEAR_BACKWARD	=	1 << 5 
	};

public:
	/// \brief constructor
	AbstractCamera( float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& f2Speed );

	/// \brief destructor
	virtual ~AbstractCamera(){}

	/// \brief the main update function, for position etc...
	virtual void Update( float fDeltaTime ) = 0;

	/// \brief Set flags related to camera movement
	void SetFlag( CameraFlagEnum eFlag, bool bValue);

	/// \brief Add value to alpha and phi angles
	void UpdateAngles( float a_fAddToAlpha, float a_fAddToPhi );

	vec3 const& GetPos() const	{return m_f3Pos;}
	vec3 & GetPos() {return m_f3Pos;}
	vec3 const& GetAim() const {return m_f3Aim;}
	vec3 const& GetUp() const {return m_f3Up;}
	vec3 const& GetRight() const {return m_f3Right;}

	float const& GetFOV() const {return m_fFOV;}
	float const& GetNear() const {return m_fNear;}
	float const& GetFar() const {return m_fFar;}

	float const& GetRY() const {return m_fRY;}
	float const& GetRX() const {return m_fRX;}

	vec4 const GetDofParams() const {return vec4( m_f4DofParams.x + m_fDofOffset, m_f4DofParams.y + m_fDofOffset,m_f4DofParams.z + m_fDofOffset, m_f4DofParams.w );}
	
	virtual float4x4 const GetViewMatrix() const = 0;
	
	virtual void LookAt() = 0;

protected:
	vec3& _GrabPos(){return m_f3Pos;}
	vec3& _GrabAim(){return m_f3Aim;}
	vec3& _GrabUp(){return m_f3Up;}

	/// \brief Calculate new Position from updated parameters
	virtual void _InternalUpdate() = 0;

protected:
	/// Angle : rotation around Up axis
	float m_fRY;
	/// Angle : rotation around Right Axis
	float m_fRX;

	/// Moving speed into space
	float m_fPositionSpeed;

	/// Mouse speed (sensibility)
	float m_fRotationSpeed;

	/// The position Vector
	/// \todo On next implementation, we should use a full matrix instead of separate vector
	vec3 m_f3Pos;
	vec3 m_f3Aim;
	vec3 m_f3Up;
	vec3 m_f3Right;

	float	m_fFOV;
	float	m_fNear;
	float	m_fFar;

	/// Depth of Field parameters
	/// x = near blur; y = focal plane; z = far blur; w = blurriness cutoff
	vec4	m_f4DofParams;
	float	m_fDofOffset;

	/// These flags are used when a key is pressed, in order to avoid keyboard repeat delay
	int m_iFlags;
};


#endif //__ABSTRACTCAMERA_H__