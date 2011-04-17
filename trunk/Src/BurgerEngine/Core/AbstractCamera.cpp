#include "AbstractCamera.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractCamera::AbstractCamera( float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& fSpeed )
	: m_fFOV( fFOV )
	, m_fNear( 0.1f )
	, m_fFar( 10000.0f )
	, m_f3Pos(f3Pos)
	, m_fRX( f2Rotation.x)
	, m_fRY( f2Rotation.y )
	, m_f4DofParams ( f4DofParams )
	, m_fDofOffset ( 0.0f )
	, m_fPositionSpeed( fSpeed.x )
	, m_fRotationSpeed( fSpeed.y )
	, m_iFlags( 0 )
{
	m_f3Up = vec3( 0.0f, 1.0f, 0.0f );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void AbstractCamera::UpdateAngles( float a_fAddToY, float a_fAddToX )
{
	m_fRY += a_fAddToY * m_fRotationSpeed;
	m_fRX += a_fAddToX * m_fRotationSpeed;

	m_fRX = m_fRX > 90.0f ? 90.0f : m_fRX;
	m_fRX = m_fRX < -90.0f ? -90.0f : m_fRX;
	
	if(m_fRY > 360.0f || m_fRY < -360.0f )
	{
		m_fRY = 0.0f; 
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void AbstractCamera::SetFlag( CameraFlagEnum eFlag, bool bValue )
{
	if( bValue )
		m_iFlags |= eFlag;
	else
		m_iFlags ^= eFlag;
}