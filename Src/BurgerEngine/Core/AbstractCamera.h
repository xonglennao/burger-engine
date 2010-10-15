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
		 E_CAMERA_FORWARD
		,E_CAMERA_BACKWARD
		,E_CAMERA_LEFT
		,E_CAMERA_RIGHT
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
	vec3 const& GetAim() const {return m_f3Aim;}
	vec3 const& GetUp() const {return m_f3Up;}
	vec3 const& GetRight() const {return m_f3Right;}

	float const& GetFOV() const {return m_fFOV;}
	float const& GetNear() const {return m_fNear;}
	float const& GetFar() const {return m_fFar;}
	
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

};


#endif //__ABSTRACTCAMERA_H__