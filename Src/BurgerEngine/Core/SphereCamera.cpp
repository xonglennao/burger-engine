#include "SphereCamera.h"
#include "BurgerEngine/Graphics/CommonGraphics.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

SphereCamera::SphereCamera( float fRadius, float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& fSpeed )
	: AbstractCamera( fFOV, f3Pos, f2Rotation, f4DofParams, fSpeed )
	, m_fRadius( fRadius )
	, m_f3CenterPosition(f3Pos)
{
};

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SphereCamera::_UpdatePosition( float fDeltaTime )
{
	float fMovingSpeed = m_fPositionSpeed * fDeltaTime;
	
	if( m_iFlags & E_CAMERA_FORWARD)
	{
		m_fRadius -= fMovingSpeed;
		m_fRadius = m_fRadius < 0.0f ?  0.0f : m_fRadius;
	}
	if( m_iFlags & E_CAMERA_BACKWARD )
	{
		m_fRadius += fMovingSpeed;
	}
	if( m_iFlags & E_DOF_NEAR_FORWARD )
	{
		m_fDofOffset += fMovingSpeed;
	}
	if( m_iFlags & E_DOF_NEAR_BACKWARD )
	{
		m_fDofOffset -= fMovingSpeed;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SphereCamera::_UpdatePositionAnalog( float fDeltaTime )
{
	
	vec4 f4Right = normalize( rotateY( -m_fRY*DEG_TO_RAD ) * vec4(-1.0f,0.0f,0.0f,1.0f) );
	f4Right /= f4Right.w;
	vec4 f4Direction = normalize( rotateY( -m_fRY*DEG_TO_RAD ) * vec4(0.0f,0.0f,-1.0f,1.0f) );
	f4Direction /= f4Direction.w;


	vec3 truc = (f4Direction.xyz()*m_fAnalogY);
	truc -= (f4Right.xyz()*m_fAnalogX);

	float bababa = min(length(truc), 1.0f);
	float beeeeee = pow(bababa,3);

	truc *= beeeeee;
	m_f3CenterPosition += truc;
	
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SphereCamera::_InternalUpdate()
{
	float fCosX = cosf( m_fRX );
	
	m_f3Pos.x = sinf( m_fRY ) * fCosX;
	m_f3Pos.y = -sinf( m_fRX );
	m_f3Pos.z = cos( m_fRY ) * fCosX;

	//Cross product
	vec3& rf3Up = _GrabUp();

	if( m_fRX > -PI_BY_2 && m_fRX < PI_BY_2 )
	{
		m_f3Right = cross( rf3Up, m_f3Pos);
	}
	else
	{
		m_f3Right = cross( m_f3Pos, rf3Up);
	}


	//normalize
	m_f3Right = normalize( m_f3Right );

	m_f3Pos *= m_fRadius;
	m_f3Pos += m_f3CenterPosition;

	m_mViewMatrix = rotateXY( -m_fRX, m_fRY ) * translate( -m_f3Pos.x, -m_f3Pos.y, -m_f3Pos.z );
}

const float4x4& SphereCamera::GetViewMatrix() const
{
	return m_mViewMatrix;
}