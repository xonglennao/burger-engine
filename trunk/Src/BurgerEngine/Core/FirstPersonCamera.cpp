#include "FirstPersonCamera.h"
#include "BurgerEngine/Graphics/CommonGraphics.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

FirstPersonCamera::FirstPersonCamera( float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& fSpeed  )
	: AbstractCamera( fFOV, f3Pos, f2Rotation, f4DofParams, fSpeed )
{			
};

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void FirstPersonCamera::Update( float fDeltaTime )
{
	vec3& rf3Pos = _GrabPos();

	float fMovingSpeed = m_fPositionSpeed * fDeltaTime * 1000.0f;
	
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

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void FirstPersonCamera::_InternalUpdate()
{
	float fRX = m_fRX * DEG_TO_RAD;
	float fRY = m_fRY * DEG_TO_RAD;

	float fCosX = cosf( fRX );
	m_f3Direction.x = fCosX * -sinf( fRY );
	m_f3Direction.y = sinf( fRX );
	m_f3Direction.z = fCosX * -cosf( fRY );

	//Cross product
	vec3& rf3Up = _GrabUp();
	if( m_fRX > -90.0f && m_fRX < 90.0f )
	{
		m_f3Right = cross( rf3Up, m_f3Direction);
	}
	else
	{
		m_f3Right = cross( m_f3Direction, rf3Up);
	}
	//normalize
	m_f3Right = normalize( m_f3Right );
}

void FirstPersonCamera::LookAt()
{
	glRotatef( -m_fRX, 1.0f, 0.0f, 0.0f );
	glRotatef( -m_fRY, 0.0f, 1.0f, 0.0f );
	glTranslatef( -m_f3Pos.x, -m_f3Pos.y, -m_f3Pos.z );
}

float4x4 const FirstPersonCamera::GetViewMatrix() const
{
	return rotateXY( -m_fRX*DEG_TO_RAD, m_fRY*DEG_TO_RAD ) * translate( -m_f3Pos.x, -m_f3Pos.y, -m_f3Pos.z );
}