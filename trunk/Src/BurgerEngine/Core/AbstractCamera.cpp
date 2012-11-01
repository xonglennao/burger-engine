#include "AbstractCamera.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractCamera::AbstractCamera( float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& fSpeed )
	: m_fFOV( fFOV )
	, m_fNear( 0.1f )
	, m_fFar( 1000.0f )
	, m_f3Pos(f3Pos)
	, m_fRX( f2Rotation.x)
	, m_fRY( f2Rotation.y )
	, m_f4DofParams ( f4DofParams )
	, m_fDofOffset ( 0.0f )
	, m_fPositionSpeed( fSpeed.x )
	, m_fAnalogX(0.0f)
	, m_fAnalogY(0.0f)
	, m_fAnalogRX(0.0f)
	, m_fAnalogRY(0.0f)
	, m_fRotationSpeed( fSpeed.y )
	, m_iFlags( 0 )
	, m_bNeedsUpdate( true )
{
	m_f3Up = vec3( 0.0f, 1.0f, 0.0f );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void AbstractCamera::UpdateAngles( float a_fAddToY, float a_fAddToX )
{
	m_fRY += a_fAddToY * DEG_TO_RAD * m_fRotationSpeed;
	m_fRY = fmod(m_fRY + PI_X_2, PI_X_2);

	m_fRX += a_fAddToX * DEG_TO_RAD * m_fRotationSpeed;
	m_fRX = m_fRX > PI_BY_2 ? PI_BY_2 : m_fRX;
	m_fRX = m_fRX < -PI_BY_2 ? -PI_BY_2 : m_fRX;

	m_bNeedsUpdate = true;
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

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void AbstractCamera::Update( float fDeltaTime )
{
	if(m_iFlags)
	{
		_UpdatePosition(fDeltaTime);
	}
	else
	{
		_UpdatePositionAnalog(fDeltaTime);
	}

	if( m_bNeedsUpdate )
	{
		_InternalUpdate();
	}
}