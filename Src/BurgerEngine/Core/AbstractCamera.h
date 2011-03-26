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
	AbstractCamera( float fFOV, float fNear, float fFar );

	/// \brief destructor
	virtual ~AbstractCamera(){}

	/// \brief the main update function, for position etc...
	virtual void Update( float fDeltaTime ) = 0;

	/// \brief Initialize Camera (set up callback)
	virtual void Initialize() = 0;

	/// \brief Terminate Camera (free callback)
	virtual void Terminate() = 0;

	/// \brief Set flags related to camera movement
	virtual void SetFlag( CameraFlagEnum eFlag, bool bValue){};

	/// \brief Add value to alpha and phi angles
	virtual void UpdateAngles( float a_fAddToAlpha, float a_fAddToPhi ) = 0;

	vec3 const& GetPos() const	{return m_f3Pos;}
	vec3 & GetPos() {return m_f3Pos;}
	vec3 const& GetAim() const {return m_f3Aim;}
	vec3 const& GetUp() const {return m_f3Up;}
	vec3 const& GetRight() const {return m_f3Right;}

	float const& GetFOV() const {return m_fFOV;}
	float const& GetNear() const {return m_fNear;}
	float const& GetFar() const {return m_fFar;}

	float const& GetRY() const {return m_fAlpha;}
	float const& GetRX() const {return m_fPhi;}

	vec4 const GetDofParams() const {return vec4( m_f3DofParams.x + m_fDofOffset, m_f3DofParams.y + m_fDofOffset,m_f3DofParams.z + m_fDofOffset, m_f3DofParams.w );}
	
	void LookAt();

protected:
	vec3& _GrabPos(){return m_f3Pos;}
	vec3& _GrabAim(){return m_f3Aim;}
	vec3& _GrabUp(){return m_f3Up;}

protected:
	/// Angle : rotation around Up axis
	float m_fAlpha;
	/// Angle : rotation around Right Axis
	float m_fPhi;	

protected:
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
	vec4	m_f3DofParams;
	float	m_fDofOffset;

};


#endif //__ABSTRACTCAMERA_H__