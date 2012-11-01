/*************************************
*
*		BurgerEngine Project
*		
*		Created :	16/02/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __ENNEMYCOMPONENT_H__
#define __ENNEMYCOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"

class MovementComponent;

class EnnemyComponent : public AbstractComponent
{
public:
	/// \brief constructor
	EnnemyComponent(CompositeComponent* a_pParent = NULL);

	/// \brief Destructor
	~EnnemyComponent();

	/// \brief Load the component using a XML Node
	/// override
	void Initialize(TiXmlElement const& a_rParameters);

	void Update( float fFrameTime, float fElapsedTime );
private:

	void Respawn();
	
	MovementComponent* m_pMovementComponent;

	float m_fSpeed;

	/// Angle : rotation around Up axis
	float	m_fRY;
	/// Angle : rotation around Right Axis
	float	m_fRX;
};

#endif //__ENNEMYCOMPONENT_H__