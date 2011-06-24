/*************************************
*
*		BurgerEngine Project
*		
*		Created :	14/06/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __MOVEMENTHACKERCOMPONENT_H__
#define __MOVEMENTHACKERCOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"

class MovementHackerComponent : public AbstractComponent
{
public:
	/// \brief constructor
	MovementHackerComponent(CompositeComponent* a_pParent = NULL);

	/// \brief Destructor
	~MovementHackerComponent();

	/// \brief Load the component using a XML Node
	/// override
	void Initialize(TiXmlElement const& a_rParameters);
	void SetComponent( AbstractComponent* pComponent );

protected:
	/// \brief Inner update
	void _Update( float fFrameTime, float fElapsedTime );
	void UpdateValue( float * fValue, float fFrameTime, float fElapsedTime, vec3 f3SinPhaseSpeedAmplitude, vec3 f3CosPhaseSpeedAmplitude, vec3 f3ConstIncMultiplier );

private:
		void SetValues( vec3& f3SinPhaseSpeedAmplitude, vec3& f3CosPhaseSpeedAmplitude, vec3& f3ConstIncMultiplier, float fSinPhase = 0.0f, float fSinSpeed = 0.0f, float fSinAmplitude = 0.0f, float fCosPhase = 0.0f, float fCosSpeed = 0.0f, float fCosAmplitude = 0.0f, float fIncrement = 0.0f, float fLastPosMultiplier = 0.0f );
		void LoadMovementHackerValues( TiXmlElement const* pXMLNode, float * pSinPhase, float * pSinSpeed, float * pSinAmplitude, float * pCosPhase, float * pCosSpeed, float * pCosAmplitude, float * pIncrement, float * pLastPosMultiplier );

		vec3 f3XSinPhaseSpeedAmplitude;
		vec3 f3XCosPhaseSpeedAmplitude;
		vec3 f3XConstIncMultiplier;

		vec3 f3YSinPhaseSpeedAmplitude;
		vec3 f3YCosPhaseSpeedAmplitude;
		vec3 f3YConstIncMultiplier;

		vec3 f3ZSinPhaseSpeedAmplitude;
		vec3 f3ZCosPhaseSpeedAmplitude;
		vec3 f3ZConstIncMultiplier;

		vec3 f3RotationXSinPhaseSpeedAmplitude;
		vec3 f3RotationXCosPhaseSpeedAmplitude;
		vec3 f3RotationXConstIncMultiplier;

		vec3 f3RotationYSinPhaseSpeedAmplitude;
		vec3 f3RotationYCosPhaseSpeedAmplitude;
		vec3 f3RotationYConstIncMultiplier;

		vec3 f3RotationZSinPhaseSpeedAmplitude;
		vec3 f3RotationZCosPhaseSpeedAmplitude;
		vec3 f3RotationZConstIncMultiplier;

		AbstractComponent* m_pComponent;
};

#endif //__MOVEMENTHACKERCOMPONENT_H__