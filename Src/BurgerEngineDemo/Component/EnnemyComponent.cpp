#include "BurgerEngineDemo/Component/EnnemyComponent.h"
#include "BurgerEngineDemo/Component/PlayerComponent.h"
#include "BurgerEngineDemo/Manager/GameplayManager.h"
#include "BurgerEngine/Core/CompositeComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"
#include "BurgerEngineDemo/Component/MovementComponent.h"

const float ENNEMY_SPEED = 35.0f;
const float ROTATION_SPEED = 12.0f;

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
EnnemyComponent::EnnemyComponent(CompositeComponent* a_pParent) :
	 AbstractComponent(a_pParent)
	,m_fSpeed(15.0f)
	,m_fRX(0.0f)
	,m_fRY(0.0f)
	,m_pMovementComponent(NULL)
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
EnnemyComponent::~EnnemyComponent()
{
	delete m_pMovementComponent;
	m_pMovementComponent = NULL;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void EnnemyComponent::Initialize(TiXmlElement const& a_rParameters)
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

void EnnemyComponent::Respawn()
{
	vec3 f3NewPosition;
	GameplayManager::GrabInstance().GetRespawnPosition(f3NewPosition);
	GetParent()->SetPos(f3NewPosition);
}

void EnnemyComponent::Update( float fFrameTime, float fElapsedTime )
{
	PlayerComponent* pCurrentPlayer = GameplayManager::GrabInstance().GetCurrentPlayer();

	vec3 f3PlayerToEnnemy = pCurrentPlayer->GetPos()- GetParent()->GetPos();

	// not final colision detection
	//not final behavior
	if(length(f3PlayerToEnnemy) < 0.5 )
	{
		Respawn();
		return;
	}


	//basic AI behavior, chase the player
	f3PlayerToEnnemy = normalize(f3PlayerToEnnemy);
	
	float fAngle = - acos(f3PlayerToEnnemy.x) + PI_BY_2;
		
	if(f3PlayerToEnnemy.z>0.0f)
	{
		fAngle *= -1.0f;
		fAngle += 180.0f*DEG_TO_RAD;
	}
	fAngle = fmod(fAngle+PI_X_2, PI_X_2);
	m_pMovementComponent->SetValues(fAngle,ENNEMY_SPEED, ROTATION_SPEED);
	m_pMovementComponent->Update(fFrameTime, fElapsedTime);
}