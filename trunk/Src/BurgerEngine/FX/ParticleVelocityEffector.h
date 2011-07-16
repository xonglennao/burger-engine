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

/// \brief Abstract class fro different effector
class ParticleVelocityEffector : public ParticleEffector
{
public:
	void ApplyEffect(float a_fTime, ParticleManager& a_rManager);
};


#endif //__PARTICLEVELOCITYEFFECTOR_H__