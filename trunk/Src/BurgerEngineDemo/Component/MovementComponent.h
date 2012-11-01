/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/10/12
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __MOVEMENTCOMPONENT_H__
#define __MOVEMENTCOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"

class MovementComponent : public AbstractComponent
{
public:
	/// \brief constructor
	MovementComponent(CompositeComponent* a_pParent = NULL);

	/// \brief Destructor
	~MovementComponent();

	void Update( float fFrameTime, float fElapsedTime );
	void SetValues( float fDestinationRotation, float fMovementSpeed, float fRotationSpeed);

	void Initialize(TiXmlElement const& a_rParameters){};

private:
	float GetRotationDirection(float fCurrentRotation, float fDestinationRotation);
	float GetAngleDifference(float fAngle1, float fAngle2);

private:

	float m_fXSpeed;
	float m_fYSpeed;

	float	m_fCurrentRotation;
	float	m_fDestinationRotation;
	float	m_fMovementSpeed;
	float	m_fRotationSpeed;
};

#endif //__MOVEMENTCOMPONENT_H__