#include "SphereCamera.h"
#include "BurgerEngine/Graphics/CommonGraphics.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

SphereCamera::SphereCamera( float fRadius, float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& fSpeed )
	: AbstractCamera( fFOV, f3Pos, f2Rotation, f4DofParams, fSpeed )
	, m_fRadius( fRadius )
{
};

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SphereCamera::Update( float fDeltaTime )
{
	vec3& rf3Pos = _GrabPos();

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
	
	if( m_iFlags )
	{
		m_bNeedsUpdate = true;
	}

	if( m_bNeedsUpdate )
	{
		_InternalUpdate();
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SphereCamera::_InternalUpdate()
{
	float fRX = m_fRX * DEG_TO_RAD;
	float fRY = m_fRY * DEG_TO_RAD;

	float fCosX = cosf( fRX );
	m_f3PositionOnSphere.x = sinf( fRY ) * fCosX;
	m_f3PositionOnSphere.y = -sinf( fRX );
	m_f3PositionOnSphere.z = cos( fRY ) * fCosX;

	m_f3PositionOnSphere *= m_fRadius;
	m_f3PositionOnSphere += m_f3Pos;

	m_mViewMatrix = rotateXY( -m_fRX*DEG_TO_RAD, m_fRY*DEG_TO_RAD ) * translate( -m_f3PositionOnSphere.x, -m_f3PositionOnSphere.y, -m_f3PositionOnSphere.z );
}

const float4x4& SphereCamera::GetViewMatrix() const
{
	return m_mViewMatrix;
}