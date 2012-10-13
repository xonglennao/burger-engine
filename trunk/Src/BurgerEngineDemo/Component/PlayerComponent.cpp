#include "BurgerEngineDemo/Component/PlayerComponent.h"
#include "BurgerEngineDemo/Manager/GameplayManager.h"
#include "BurgerEngine/Core/CompositeComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

const float PLAYER_SPEED = 30.0f;

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PlayerComponent::PlayerComponent(CompositeComponent* a_pParent):
AbstractComponent(a_pParent)
{
	GameplayManager::GrabInstance().SetCurrentPlayer( this );

	m_fAnalogY = 0.0f;
	m_fAnalogX = 0.0f;
	m_fRX = 0.0f;
	m_fRY = 0.0f;

	m_fCurrentRotation = 0.0f;

	m_fSpeed = 0.0f;
	m_fJumpSpeed = 0.0f;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PlayerComponent::PlayerComponent(AbstractComponent const& a_rToCopy):
	AbstractComponent()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PlayerComponent::~PlayerComponent()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PlayerComponent::Initialize(TiXmlElement const& a_rParameters)
{
	//Retrieve position
	//First we should check is there is not an desc offset
	//to the parent node
	float x, y, z, rX, rY, rZ, scale;

	TiXmlElement const* pPositionXml = a_rParameters.FirstChildElement( "position" );
	if(pPositionXml)
	{
		//gets position & bounded volume information 
		pPositionXml->QueryFloatAttribute("x",&x);
		pPositionXml->QueryFloatAttribute("y",&y);
		pPositionXml->QueryFloatAttribute("z",&z);

		pPositionXml->QueryFloatAttribute("rX",&rX);
		pPositionXml->QueryFloatAttribute("rY",&rY);
		pPositionXml->QueryFloatAttribute("rZ",&rZ);

		pPositionXml->QueryFloatAttribute("scale",&scale);
	}

	SetPos( vec3(x,y,z) );
	SetRotation( vec3( rX, rY, rZ ) );
	SetScale( scale );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
// /!\ might be moved elsewhere
float PlayerComponent::GetRotationDirection(float fCurrentRotation, float fDestinationRotation)
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
float PlayerComponent::GetAngleDifference(float fAngle1, float fAngle2)
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


void PlayerComponent::_Update( float fFrameTime, float fElapsedTime )
{
	// !!!!!!!!! /!\ !!!!!!!!!!
	// this code should be moved into a new component
	// so it can be used for anything that has the ability to move
	// !!!!!!!!! /!\ !!!!!!!!!!

	// m_fRY is the angle given by the camera
	// it gives us the direction away from the screen

	float fDestRotation = m_fRY;

	//we need to add the value of the angle given by the joystick
	float fAngleInc = 0.0f;

	vec2 f2StickDirection;
	vec2 f2StickValues = vec2(m_fAnalogX,m_fAnalogY);
	
	//this gives us the normalized vector of the joystick values
	if( m_fAnalogX!= 0.0f || m_fAnalogY != 0.0f )
	{
		if( m_fAnalogX!= 0.0f &&  m_fAnalogY != 0.0f )
		{
			f2StickDirection = normalize ( f2StickValues );
		}
		else if(m_fAnalogX != 0.0f)
		{
			f2StickDirection = vec2(1.0f,0.0f);
		}
		else if(m_fAnalogY != 0.0f)
		{
			f2StickDirection = vec2(0.0f,1.0f);
		}
	}
	else
	{
		f2StickDirection = vec2(0.0f,0.0f);
	}

	//we can now compute the angle
	//if the joystick is all the way up we don't had anything to the camera angle (the players is moving away from the screen)
	// if it's all the way down it's 180° (the players is moving toward the screen)
	fAngleInc = - acos(f2StickDirection.x) + PI_BY_2;
		
	if(m_fAnalogY<0.0f)
	{
		fAngleInc *= -1.0f;
		fAngleInc+= 180.0f*DEG_TO_RAD;
	}
	
	//we're always makin sure the angles are in the interval [0:2*PI]
	fAngleInc+= PI_X_2;
	fAngleInc = fmod(fAngleInc, PI_X_2);

	fDestRotation += fAngleInc;
	fDestRotation+= PI_X_2;
	fDestRotation = fmod(fDestRotation, PI_X_2);

	//now we have the angle we should use
	//but we need to go from the current angle to that angle smoothly

	if(m_fCurrentRotation != fDestRotation)
	{
		//do we need to rotate clockwise or anti-clockwise?
		float fDirection = GetRotationDirection(m_fCurrentRotation,fDestRotation);

		//we increment the current rotation until we've reache the destination rotation
		m_fCurrentRotation += min( length(f2StickValues), 1.0f ) * fFrameTime * 12.0f * fDirection;
		m_fCurrentRotation += PI_X_2;
		m_fCurrentRotation = fmod(m_fCurrentRotation, PI_X_2);

		//if the direction changes, we've gone to far
		float fNewDirection = GetRotationDirection(m_fCurrentRotation,fDestRotation);

		if(fDirection != fNewDirection)
		{
			m_fCurrentRotation = fDestRotation;
		}
	}

	//we compute the direction vector
	m_f4Direction = normalize( rotateY( m_fCurrentRotation ) * vec4(0.0f,0.0f,-1.0f,1.0f) );
	m_f4Direction /= m_f4Direction.w;

	vec3 vPos = GetParent()->GetPos();

	//yeahh the player can also jump
	//ulgy code, it's just for fun...
	if(m_bChargeJump)
	{
		m_fJumpSpeed += fFrameTime * 5.0f;
		m_fJumpSpeed = min(m_fJumpSpeed, 1.0f);
	}
	else
	{
		vPos.y += fFrameTime * m_fJumpSpeed * 50.0f;
		m_fJumpSpeed -= fFrameTime * 2.0f;
		m_fJumpSpeed = max(m_fJumpSpeed, 0.0f);
	}

	////////// FAKE PHYSIC /////////////
	vPos.y -= fFrameTime * 15.0f;
	vPos.y = max(vPos.y, 0.0f);
	////////// FAKE PHYSIC /////////////
	
	float fCubeLength = pow(min( length(f2StickValues), 1.0f ),3.0f);
	vPos += m_f4Direction.xyz() * fCubeLength * PLAYER_SPEED * fFrameTime;

	GetParent()->SetPos(vPos);

	GetParent()->SetRotation(vec3(0.0f, m_fCurrentRotation*RAD_TO_DEG, 0.0f) );
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PlayerComponent::UpdateAngles( float a_fAddToY, float a_fAddToX )
{
	m_fRY += a_fAddToY * DEG_TO_RAD * 0.05f;
	m_fRY = fmod(m_fRY + PI_X_2, PI_X_2);
}

void PlayerComponent::ChargeJump( bool bCharge )
{
	if(m_fJumpSpeed == 0.0f && bCharge )
	{
		m_bChargeJump = true;
	}
	else if(!bCharge)
	{
		m_bChargeJump = false;
	}
}