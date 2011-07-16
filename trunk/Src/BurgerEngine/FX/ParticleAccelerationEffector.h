/*************************************
*
*		BurgerEngine Project
*		
*		Created :	11/06/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __PARTICLEACCELERATIONEFFECTOR_H__
#define __PARTICLEACCELERATIONEFFECTOR_H__

#include "BurgerEngine/fx/ParticleEffector.h"
#include "BurgerEngine/External/Math/Vector.h"

/// \brief Abstract class fro different effector
class ParticleAccelerationEffector : public ParticleEffector
{
public:
	ParticleAccelerationEffector():
		m_f3InitAcceleration(0.0,-1.0,0.0)
		{}
	void ApplyEffect(float a_fTime, ParticleManager& a_rManager);

	void SetAcceleration(vec3 const& a_rAcceleration){m_f3InitAcceleration = a_rAcceleration;}
private:
	vec3 m_f3InitAcceleration;
};


#endif //__PARTICLEACCELERATIONEFFECTOR_H__