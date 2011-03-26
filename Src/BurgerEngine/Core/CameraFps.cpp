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

	m_fAlpha = 0.0f;
	m_fPhi = 0.0f;
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

	m_fPhi = m_fPhi > 90.0f ? 90.0f : m_fPhi;
	m_fPhi = m_fPhi < -90.0f ? -90.0f : m_fPhi;
	
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

	float fRX = m_fPhi* DEG_TO_RAD;
	float fRY = m_fAlpha* DEG_TO_RAD;

	rf3Direction.x = cosf( fRX ) * -sinf( fRY );
	rf3Direction.y = sinf( fRX );
	rf3Direction.z = cosf( fRX ) * -cosf( fRY );

	//Cross product
	vec3& rf3Up = _GrabUp();
	//rf3Up = ( rotateX( fRX )* vec4(0.0f,1.0f,0.0f,1.0f) ).xyz();
	if( m_fPhi > -90.0f && m_fPhi < 90.0f )
	{
		m_f3Right = cross( rf3Up, rf3Direction);
	}
	else
	{
		m_f3Right = cross( rf3Direction, rf3Up);
	}


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

