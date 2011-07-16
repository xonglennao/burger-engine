/*************************************
*
*		BurgerEngine Project
*		
*		Created :	31/05/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __PARTICLEEFFECTOR_H__
#define __PARTICLEEFFECTOR_H__

#include "BurgerEngine/fx/FXCommon.h"

/// \brief Abstract class fro different effector
class ParticleEffector
{
public:
	virtual void ApplyEffect(float a_fTime, ParticleManager& a_rManager) = 0;
};


#endif //__PARTICLEEFFECTOR_H__