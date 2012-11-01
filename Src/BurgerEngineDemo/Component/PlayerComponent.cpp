#include "BurgerEngineDemo/Component/PlayerComponent.h"
#include "BurgerEngineDemo/Manager/GameplayManager.h"
#include "BurgerEngine/Core/CompositeComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"
#include "BurgerEngineDemo/Component/MovementComponent.h"

const float PLAYER_SPEED = 30.0f;
const float ROTATION_SPEED = 12.0f;

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PlayerComponent::PlayerComponent(CompositeComponent* a_pParent) :
	 AbstractComponent(a_pParent)
	,m_fAnalogY(0.0f)
	,m_fAnalogX(0.0f)
	,m_fRX(0.0f)
	,m_fRY(0.0f)
	,m_pMovementComponent(NULL)
                                                                                                                                                                                                 {
	
	GameplayManager::GrabInstance().SetCurrentPlayer( this );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
PlayerComponent::~PlayerComponent()
{
	delete m_pMovementComponent;
	m_pMovementComponent = NULL;
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

	m_pMovementComponent = new MovementComponent( GetParent() );
}

void PlayerComponent::Update( float fFrameTime, float fElapsedTime )
{
	if( m_fAnalogX == 0.0f && m_fAnalogY == 0.0f )
	{
		return;
	}

	// m_fRY is the angle given by the camera
	// it gives us the direction away from the screen
	float fDestRotation = m_fRY;

	//we need to add the value of the angle given by the joystick
	float fAngleInc = 0.0f;

	vec2 f2StickDirection;
	vec2 f2StickValues = vec2(m_fAnalogX,m_fAnalogY);
	
	//this gives us the normalized vector of the joystick values
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


	//we can now compute the angle
	//if the joystick is all the way up we don't had anything to the camera angle (the players is moving away from the screen)
	// if it's all the way down it's 180° (the players is moving toward the screen)
	fAngleInc = - acos(f2StickDirection.x) + PI_BY_2;
		
	if(m_fAnalogY<0.0f)
	{
		fAngleInc *= -1.0f;
		fAngleInc += 180.0f*DEG_TO_RAD;
	}
	
	//we're always making sure the angles are in the interval [0:2*PI]
	fAngleInc += PI_X_2;
	fAngleInc = fmod(fAngleInc, PI_X_2);

	fDestRotation += fAngleInc;
	fDestRotation += PI_X_2;
	fDestRotation = fmod(fDestRotation, PI_X_2);

	float fJoystickLength = min( length(f2StickValues), 1.0f );
	float fCubeLength = pow(fJoystickLength,3.0f);
	
	m_pMovementComponent->SetValues(fDestRotation,fCubeLength * PLAYER_SPEED, fJoystickLength*ROTATION_SPEED);
	m_pMovementComponent->Update(fFrameTime, fElapsedTime);
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void PlayerComponent::UpdateAngles( float a_fAddToY, float a_fAddToX )
{
	m_fRY += a_fAddToY * DEG_TO_RAD * 0.05f;
	m_fRY = fmod(m_fRY + PI_X_2, PI_X_2);
}