#include "BurgerEngineDemo/Component/MovementComponent.h"

#include "BurgerEngine/Core/CompositeComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

const float PLAYER_SPEED = 30.0f;
const float ROTATION_SPEED = 12.0f;

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
MovementComponent::MovementComponent(CompositeComponent* a_pParent) :
	 AbstractComponent(a_pParent)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
MovementComponent::~MovementComponent()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
float MovementComponent::GetRotationDirection(float fCurrentRotation, float fDestinationRotation)
{
	float fDirection = 1.0f;

	float fAngleDifference = fCurrentRotation - fDestinationRotation;

	if( fAngleDifference > 0.0f )
	{
		if( fDestinationRotation + PI_X_2 - fCurrentRotation > PI)
		{
			fDirection = -1.0;
		}
	}
	else
	{
		if( fCurrentRotation + PI_X_2 - fDestinationRotation < PI)
		{
			fDirection = -1.0;
		}
	}

	return fDirection;
}

// /!\ Not used at the moment but can be useful later
float MovementComponent::GetAngleDifference(float fAngle1, float fAngle2)
{
	float fDifference = 0.0f;
	float fIncrement, fScaledAngle;

	float fAngleDifference = fAngle1 - fAngle2;

	if( fAngleDifference > 0.0f )
	{
		fIncrement = PI_X_2 - fAngle1;

		fScaledAngle = fAngle2 + fIncrement;
		if( fScaledAngle > PI)
		{
			fDifference = PI_X_2 - fScaledAngle;
		}
		else
		{
			fDifference = fScaledAngle;
		}
	}
	else
	{
		fIncrement = PI_X_2 - fAngle2;

		fScaledAngle = fAngle1 + fIncrement;
		if( fScaledAngle > PI)
		{
			fDifference = PI_X_2 - fScaledAngle;
		}
		else
		{
			fDifference = fScaledAngle;
		}
	}

	return fDifference;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MovementComponent::SetValues( float fDestinationRotation, float fMovementSpeed, float fRotationSpeed)
{
	m_fDestinationRotation = fDestinationRotation;
	m_fMovementSpeed = fMovementSpeed;
	m_fRotationSpeed = fRotationSpeed;
}

void MovementComponent::Update( float fFrameTime, float fElapsedTime )
{
	//now we have the angle we should use
	//but we need to go from the current angle to that angle smoothly

	if(m_fCurrentRotation != m_fDestinationRotation)
	{
		//do we need to rotate clockwise or anti-clockwise?
		float fRotationDirection = GetRotationDirection(m_fCurrentRotation,m_fDestinationRotation);

		//we increment the current rotation until we've reache the destination rotation
		m_fCurrentRotation += m_fRotationSpeed * fFrameTime * fRotationDirection;
		m_fCurrentRotation += PI_X_2;
		m_fCurrentRotation = fmod(m_fCurrentRotation, PI_X_2);

		//if the direction changes, we've gone to far
		float fNewDirection = GetRotationDirection(m_fCurrentRotation,m_fDestinationRotation);

		if(fRotationDirection != fNewDirection)
		{
			m_fCurrentRotation = m_fDestinationRotation;
		}
	}

	//we compute the direction vector
	vec4 f4Direction = normalize( rotateY( m_fCurrentRotation ) * vec4(0.0f,0.0f,-1.0f,1.0f) );
	f4Direction /= f4Direction.w;

	GetParent()->SetPos(GetParent()->GetPos() + f4Direction.xyz() * m_fMovementSpeed * fFrameTime);
	GetParent()->SetRotation(vec3(0.0f, m_fCurrentRotation*RAD_TO_DEG, 0.0f) );
}