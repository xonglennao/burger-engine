#include "CameraFps.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Input/EventManager.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void CameraFps::Initialize()
{
	//AbstractCamera::Initialize();

	/// \todo Get back init parameters
	m_fMovingSpeed = 0.02f;

	m_fMouseSpeed = 0.1f;

	m_fAlpha=0;
	m_fPhi=-90;

	m_bForward = false;
	m_bBackward = false;
	m_bLeft = false;
	m_bRight = false;

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
	
	if( m_bForward )
	{
		rf3Pos.x += fMovingSpeed * m_f3Direction.x;
		rf3Pos.y += fMovingSpeed * m_f3Direction.y;
		rf3Pos.z += fMovingSpeed * m_f3Direction.z;
	}
	if( m_bBackward )
	{
		rf3Pos.x -= fMovingSpeed * m_f3Direction.x;
		rf3Pos.y -= fMovingSpeed * m_f3Direction.y;
		rf3Pos.z -= fMovingSpeed * m_f3Direction.z;	
	}
	if( m_bLeft )
	{
		rf3Pos.x += fMovingSpeed * m_f3Right.x;
		rf3Pos.z += fMovingSpeed * m_f3Right.z;	
	}
	if( m_bRight )
	{
		rf3Pos.x -= fMovingSpeed * m_f3Right.x;
		rf3Pos.z -= fMovingSpeed * m_f3Right.z;
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

	rf3Direction.x = sinf(m_fPhi*M_PI/180.0f)*sin(m_fAlpha*M_PI/180.0f);
	rf3Direction.y = cosf(m_fPhi*M_PI/180.0f);
	rf3Direction.z = sinf(m_fPhi*M_PI/180.0f)*cos(m_fAlpha*M_PI/180.0f);

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
	switch( eFlag )
	{
	case E_CAMERA_FORWARD:
		m_bForward = bValue;
		break;
	case E_CAMERA_BACKWARD:
		m_bBackward = bValue;	
		break;
	case E_CAMERA_LEFT:
		m_bLeft = bValue;
		break;
	case E_CAMERA_RIGHT:
		m_bRight = bValue;
		break;
	}
}

