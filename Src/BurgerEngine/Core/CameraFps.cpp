#include "CameraFps.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Input/EventManager.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

	CameraFps::CameraFps( float fFOV, float fNear, float fFar )
		: AbstractCamera( fFOV, fNear, fFar )
		, m_iFlags( 0 )
	{
			
	};
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::Initialize()
{
	//AbstractCamera::Initialize();

	/// \todo Get back init parameters
	m_fMovingSpeed = 0.05f;

	m_fMouseSpeed = 0.1f;

	m_fAlpha=0;
	m_fPhi=-90;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::Terminate()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::Update( float fDeltaTime )
{
	vec3& rf3Pos = _GrabPos();

	float fMovingSpeed = m_fMovingSpeed * fDeltaTime;
	
	if( m_iFlags & E_CAMERA_FORWARD)
	{
		rf3Pos.x += fMovingSpeed * m_f3Direction.x;
		rf3Pos.y += fMovingSpeed * m_f3Direction.y;
		rf3Pos.z += fMovingSpeed * m_f3Direction.z;
	}
	if( m_iFlags & E_CAMERA_BACKWARD )
	{
		rf3Pos.x -= fMovingSpeed * m_f3Direction.x;
		rf3Pos.y -= fMovingSpeed * m_f3Direction.y;
		rf3Pos.z -= fMovingSpeed * m_f3Direction.z;	
	}
	if( m_iFlags & E_CAMERA_LEFT )
	{
		rf3Pos.x += fMovingSpeed * m_f3Right.x;
		rf3Pos.z += fMovingSpeed * m_f3Right.z;	
	}
	if( m_iFlags & E_CAMERA_RIGHT )
	{
		rf3Pos.x -= fMovingSpeed * m_f3Right.x;
		rf3Pos.z -= fMovingSpeed * m_f3Right.z;
	}
	if( m_iFlags & E_DOF_NEAR_FORWARD )
	{
		m_fDofOffset += fMovingSpeed;
	}
	if( m_iFlags & E_DOF_NEAR_BACKWARD )
	{
		m_fDofOffset -= fMovingSpeed;
	}

	_InternalUpdate();
}

void CameraFps::UpdateAngles( float a_fAddToAlpha, float a_fAddToPhi )
{
	m_fAlpha += a_fAddToAlpha * m_fMouseSpeed;
	m_fPhi += a_fAddToPhi * m_fMouseSpeed;

	m_fPhi = m_fPhi > -1.0f ? -1.0f : m_fPhi;
	m_fPhi = m_fPhi < -179.0f ? -179.0f : m_fPhi;
	
	if(m_fAlpha > 360.0f || m_fAlpha < -360.0f )
	{
		m_fAlpha = 0.0f; 
	}

	_InternalUpdate();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::_InternalUpdate()
{
	vec3& rf3Direction = _GrabDirection();

	rf3Direction.x = sinf(m_fPhi*(float)M_PI/180.0f)*sinf(m_fAlpha*(float)M_PI/180.0f);
	rf3Direction.y = cosf(m_fPhi*(float)M_PI/180.0f);
	rf3Direction.z = sinf(m_fPhi*(float)M_PI/180.0f)*cosf(m_fAlpha*(float)M_PI/180.0f);

	//Cross product
	vec3& rf3Up = _GrabUp();
	m_f3Right = cross( rf3Up, rf3Direction);

	//normalize
	m_f3Right = normalize( m_f3Right );

	vec3& rf3Aim = _GrabAim();
	vec3& rf3Pos = _GrabPos();

	rf3Aim = rf3Pos + rf3Direction;
}

void CameraFps::SetFlag( CameraFlagEnum eFlag, bool bValue )
{
	if( bValue )
		m_iFlags |= eFlag;
	else
		m_iFlags ^= eFlag;
}

