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
#ifndef __PARTICLEVELOCITYEFFECTOR_H__
#define __PARTICLEVELOCITYEFFECTOR_H__

#include "BurgerEngine/fx/ParticleEffector.h"
#include "BurgerEngine/External/Math/Vector.h"

/// \brief Abstract class fro different effector
class ParticleVelocityEffector : public ParticleEffector
{
public:
	ParticleVelocityEffector():
		m_f3InitMaxVelocity(0.0,-1.0,0.0),
		m_f3InitMinVelocity(0.0,-1.0,0.0)
		{}

	void ApplyEffect(float a_fTime, ParticleManager& a_rManager);
	void SetVelocity(vec3 const& a_rVelocity){m_f3InitMaxVelocity = a_rVelocity;m_f3InitMinVelocity = a_rVelocity;}
	void SetVelocity(vec3 const& a_rMin, vec3 const& a_rMax){m_f3InitMaxVelocity = a_rMax;m_f3InitMinVelocity = a_rMin;}
private:
	vec3 m_f3InitMaxVelocity;
	vec3 m_f3InitMinVelocity;
};


#endif //__PARTICLEVELOCITYEFFECTOR_H__