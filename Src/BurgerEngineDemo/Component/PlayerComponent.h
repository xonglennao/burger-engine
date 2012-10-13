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
#ifndef __PLAYERCOMPONENT_H__
#define __PLAYERCOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"

class SceneMesh;

class PlayerComponent : public AbstractComponent
{
public:
	/// \brief constructor
	PlayerComponent(CompositeComponent* a_pParent = NULL);

	/// \brief Copy constructor //clone???
	PlayerComponent(AbstractComponent const& a_rToCopy);

	/// \brief Destructor
	~PlayerComponent();

	/// \brief Load the component using a XML Node
	/// override
	void Initialize(TiXmlElement const& a_rParameters);

	void SetAnalogX(float fValue){ m_fAnalogX = fValue; };
	void SetAnalogY(float fValue){ m_fAnalogY = fValue; };

	/// \brief Add value to alpha and phi angles
	void UpdateAngles( float a_fAddToAlpha, float a_fAddToPhi );

	void ChargeJump( bool bCharge );

protected:
	/// \brief Inner update
	void _Update( float fFrameTime, float fElapsedTime );

private:
	float GetRotationDirection(float fCurrentRotation, float fDestinationRotation);
	float GetAngleDifference(float fAngle1, float fAngle2);

private:
	/// Analog values for motion and rotation 
	float m_fAnalogX;
	float m_fAnalogY;

	/// Angle : rotation around Up axis
	float	m_fRY;
	/// Angle : rotation around Right Axis
	float	m_fRX;

	vec4	m_f4Direction;

	float	m_fCurrentRotation;

	float	m_fSpeed;

	bool	m_bChargeJump;
	float	m_fJumpSpeed;
};

#endif //__PLAYERCOMPONENT_H__