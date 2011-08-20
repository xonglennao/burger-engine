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
		m_f3InitMaxAcceleration(0.0,-1.0,0.0),
		m_f3InitMinAcceleration(0.0,-1.0,0.0)
		{}
	void ApplyEffect(float a_fTime, ParticleManager& a_rManager);

	void SetAcceleration(vec3 const& a_rAcceleration){m_f3InitMaxAcceleration = a_rAcceleration;m_f3InitMinAcceleration = a_rAcceleration;}
	void SetAcceleration(vec3 const& a_rMin, vec3 const& a_rMax){m_f3InitMaxAcceleration = a_rMax;m_f3InitMinAcceleration = a_rMin;}
private:
	vec3 m_f3InitMaxAcceleration;
	vec3 m_f3InitMinAcceleration;
};


#endif //__PARTICLEACCELERATIONEFFECTOR_H__