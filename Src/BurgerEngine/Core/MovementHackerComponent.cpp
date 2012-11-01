#include "BurgerEngine/Core/MovementHackerComponent.h"
#include "BurgerEngine/Core/ObjectFactory.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/CompositeComponent.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
MovementHackerComponent::MovementHackerComponent(CompositeComponent* a_pParent)
	: AbstractComponent(a_pParent)
	, m_pComponent( NULL )
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
MovementHackerComponent::~MovementHackerComponent()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MovementHackerComponent::Initialize(TiXmlElement const& a_rParameters)
{
	//Position paramaters
	TiXmlElement const*  pPosX = a_rParameters.FirstChildElement( "posx" );
	if( pPosX )
	{
		float fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastPosMultiplier;
		LoadMovementHackerValues(pPosX, &fSinPhase, &fSinSpeed, &fSinAmplitude, &fCosPhase, &fCosSpeed, &fCosAmplitude, &fIncrement, &fLastPosMultiplier );
		SetValues( f3XSinPhaseSpeedAmplitude, f3XCosPhaseSpeedAmplitude, f3XConstIncMultiplier, fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastPosMultiplier );
	}
	else
	{
		SetValues( f3XSinPhaseSpeedAmplitude, f3XCosPhaseSpeedAmplitude, f3XConstIncMultiplier );
	}

	TiXmlElement const* pPosY = a_rParameters.FirstChildElement( "posy" );
	if( pPosY )
	{
		float fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastPosMultiplier;
		LoadMovementHackerValues(pPosY, &fSinPhase, &fSinSpeed, &fSinAmplitude, &fCosPhase, &fCosSpeed, &fCosAmplitude, &fIncrement, &fLastPosMultiplier );
		SetValues( f3YSinPhaseSpeedAmplitude, f3YCosPhaseSpeedAmplitude, f3YConstIncMultiplier, fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastPosMultiplier );
	}
	else
	{
		SetValues( f3YSinPhaseSpeedAmplitude, f3YCosPhaseSpeedAmplitude, f3YConstIncMultiplier );
	}

	TiXmlElement const* pPosZ= a_rParameters.FirstChildElement( "posz" );
	if( pPosZ )
	{
		float fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastPosMultiplier;
		LoadMovementHackerValues(pPosZ, &fSinPhase, &fSinSpeed, &fSinAmplitude, &fCosPhase, &fCosSpeed, &fCosAmplitude, &fIncrement, &fLastPosMultiplier );
		SetValues( f3ZSinPhaseSpeedAmplitude, f3ZCosPhaseSpeedAmplitude, f3ZConstIncMultiplier, fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastPosMultiplier );
	}
	else
	{
		SetValues( f3ZSinPhaseSpeedAmplitude, f3ZCosPhaseSpeedAmplitude, f3ZConstIncMultiplier );
	}

	//Rotation paramaters
	TiXmlElement const* pRotationX = a_rParameters.FirstChildElement( "rotationx" ) ;
	if( pRotationX )
	{
		float fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastRotationMultiplier;
		LoadMovementHackerValues(pRotationX, &fSinPhase, &fSinSpeed, &fSinAmplitude, &fCosPhase, &fCosSpeed, &fCosAmplitude, &fIncrement, &fLastRotationMultiplier );
		SetValues( f3RotationXSinPhaseSpeedAmplitude, f3RotationXCosPhaseSpeedAmplitude, f3RotationXConstIncMultiplier, fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastRotationMultiplier );
	}
	else
	{
		SetValues( f3RotationXSinPhaseSpeedAmplitude, f3RotationXCosPhaseSpeedAmplitude, f3RotationXConstIncMultiplier );
	}

	TiXmlElement const* pRotationY = a_rParameters.FirstChildElement( "rotationy" );
	if( pRotationY )
	{
		float fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastRotationMultiplier;
		LoadMovementHackerValues(pRotationY, &fSinPhase, &fSinSpeed, &fSinAmplitude, &fCosPhase, &fCosSpeed, &fCosAmplitude, &fIncrement, &fLastRotationMultiplier );
		SetValues( f3RotationYSinPhaseSpeedAmplitude, f3RotationYCosPhaseSpeedAmplitude, f3RotationYConstIncMultiplier, fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastRotationMultiplier );
	}
	else
	{
		SetValues( f3RotationYSinPhaseSpeedAmplitude, f3RotationYCosPhaseSpeedAmplitude, f3RotationYConstIncMultiplier );
	}

	TiXmlElement const* pRotationZ = a_rParameters.FirstChildElement( "rotationz" );
	if( pRotationZ )
	{
		float fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastRotationMultiplier;
		LoadMovementHackerValues(pRotationZ, &fSinPhase, &fSinSpeed, &fSinAmplitude, &fCosPhase, &fCosSpeed, &fCosAmplitude, &fIncrement, &fLastRotationMultiplier );
		SetValues( f3RotationZSinPhaseSpeedAmplitude, f3RotationZCosPhaseSpeedAmplitude, f3RotationZConstIncMultiplier, fSinPhase, fSinSpeed, fSinAmplitude, fCosPhase, fCosSpeed, fCosAmplitude, fIncrement, fLastRotationMultiplier );
	}
	else
	{
		SetValues( f3RotationZSinPhaseSpeedAmplitude, f3RotationZCosPhaseSpeedAmplitude, f3RotationZConstIncMultiplier );
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MovementHackerComponent::UpdateValue( float * fValue, float fFrameTime, float fElapsedTime, vec3 f3SinPhaseSpeedAmplitude, vec3 f3CosPhaseSpeedAmplitude, vec3 f3ConstIncMultiplier )
{
	float fSinValue = sinf( fElapsedTime * f3SinPhaseSpeedAmplitude.y + f3SinPhaseSpeedAmplitude.x ) * f3SinPhaseSpeedAmplitude.z;
	float fCosValue = cosf( fElapsedTime * f3CosPhaseSpeedAmplitude.y + f3CosPhaseSpeedAmplitude.x ) * f3CosPhaseSpeedAmplitude.z;
	*fValue = ( f3ConstIncMultiplier.z * *fValue ) + ( fFrameTime * f3ConstIncMultiplier.y ) + fSinValue + fCosValue;
	if( f3SinPhaseSpeedAmplitude.y || f3CosPhaseSpeedAmplitude.y )
	{
		*fValue += f3ConstIncMultiplier.x;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MovementHackerComponent::Update( float fFrameTime, float fElapsedTime )
{
	vec3 f3Pos = m_pComponent->GetPos();
	UpdateValue( &f3Pos.x, fFrameTime, fElapsedTime, f3XSinPhaseSpeedAmplitude, f3XCosPhaseSpeedAmplitude, f3XConstIncMultiplier);
	UpdateValue( &f3Pos.y, fFrameTime, fElapsedTime, f3YSinPhaseSpeedAmplitude, f3YCosPhaseSpeedAmplitude, f3YConstIncMultiplier);
	UpdateValue( &f3Pos.z, fFrameTime, fElapsedTime, f3ZSinPhaseSpeedAmplitude, f3ZCosPhaseSpeedAmplitude, f3ZConstIncMultiplier);
	m_pComponent->SetPos( f3Pos );

	vec3 f3Rotation = m_pComponent->GetRotation();
	UpdateValue( &f3Rotation.x, fFrameTime, fElapsedTime, f3RotationXSinPhaseSpeedAmplitude, f3RotationXCosPhaseSpeedAmplitude, f3RotationXConstIncMultiplier);
	UpdateValue( &f3Rotation.y, fFrameTime, fElapsedTime, f3RotationYSinPhaseSpeedAmplitude, f3RotationYCosPhaseSpeedAmplitude, f3RotationYConstIncMultiplier);
	UpdateValue( &f3Rotation.z, fFrameTime, fElapsedTime, f3RotationZSinPhaseSpeedAmplitude, f3RotationZCosPhaseSpeedAmplitude, f3RotationZConstIncMultiplier);
	m_pComponent->SetRotation( f3Rotation );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MovementHackerComponent::SetValues( vec3& f3SinPhaseSpeedAmplitude, vec3& f3CosPhaseSpeedAmplitude, vec3& f3ConstIncMultiplier,float fSinPhase, float fSinSpeed, float fSinAmplitude, float fCosPhase, float fCosSpeed, float fCosAmplitude, float fIncrement, float fLastPosMultiplier )
{
	f3SinPhaseSpeedAmplitude.x = fSinPhase;
	f3SinPhaseSpeedAmplitude.y = fSinSpeed;
	f3SinPhaseSpeedAmplitude.z = fSinAmplitude;

	f3CosPhaseSpeedAmplitude.x = fCosPhase;
	f3CosPhaseSpeedAmplitude.y = fCosSpeed;
	f3CosPhaseSpeedAmplitude.z = fCosAmplitude;

	//f3ConstIncMultiplier.x is initialized when m_pComponent is set
	f3ConstIncMultiplier.y = fIncrement;
	f3ConstIncMultiplier.z = fLastPosMultiplier;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MovementHackerComponent::LoadMovementHackerValues(TiXmlElement const* pXMLNode, float * pSinPhase, float * pSinSpeed, float * pSinAmplitude, float * pCosPhase, float * pCosSpeed, float * pCosAmplitude, float * pIncrement, float * pLastPosMultiplier )
{
	pXMLNode->QueryFloatAttribute( "sinPhase", pSinPhase );
	pXMLNode->QueryFloatAttribute( "sinSpeed",  pSinSpeed );
	pXMLNode->QueryFloatAttribute( "sinAmplitude", pSinAmplitude );

	pXMLNode->QueryFloatAttribute( "cosPhase", pCosPhase );
	pXMLNode->QueryFloatAttribute( "cosSpeed", pCosSpeed );
	pXMLNode->QueryFloatAttribute( "cosAmplitude", pCosAmplitude );

	pXMLNode->QueryFloatAttribute( "increment", pIncrement );
	pXMLNode->QueryFloatAttribute( "lastPosMultiplier", pLastPosMultiplier );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void MovementHackerComponent::SetComponent( AbstractComponent* pComponent )
{
	m_pComponent = pComponent;

	vec3 f3Pos = pComponent->GetPos();
	f3XConstIncMultiplier.x = f3Pos.x;
	f3YConstIncMultiplier.x = f3Pos.y;
	f3ZConstIncMultiplier.x = f3Pos.z;

	vec3 f3Rotation = pComponent->GetRotation();
	f3RotationXConstIncMultiplier.x = f3Rotation.x;
	f3RotationYConstIncMultiplier.x = f3Rotation.y;
	f3RotationZConstIncMultiplier.x = f3Rotation.z;
}