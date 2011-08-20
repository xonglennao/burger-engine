/*************************************
*
*		BurgerEngine Project
*		
*		Created :	20/08/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __PARTICLELIFEEFFECTOR_H__
#define __PARTICLELIFEEFFECTOR_H__

#include "BurgerEngine/fx/ParticleEffector.h"
#include "BurgerEngine/External/Math/Vector.h"

/// \brief Life effector, can delete a particle from render 
class ParticleLifeEffector : public ParticleEffector
{
public:
	ParticleLifeEffector():
		m_fInitLifeMin(1.0f),
		m_fInitLifeMax(2.0f)
		{}
	void ApplyEffect(float a_fTime, ParticleManager& a_rManager);

	void SetLifeTime(float a_fMin, float a_fMax){m_fInitLifeMax = a_fMax;m_fInitLifeMin = a_fMin;}
private:
	float m_fInitLifeMin;
	float m_fInitLifeMax;
};


#endif //__PARTICLELIFEEFFECTOR_H__