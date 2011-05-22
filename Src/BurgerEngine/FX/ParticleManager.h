/*************************************
*
*		BurgerEngine Project
*		
*		Created :	22/05/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include "BurgerEngine/fx/FXCommon.h"
#include "BurgerEngine/FX/Particle.h"
#include <vector>

/// \brief Manager the memory for one layer of effect
/// It equal to one batch
class ParticleManager
{
	typedef std::vector<Particle> Particles; 
public:
	/// \brief constructor
	ParticleManager()
	  {}
	~ParticleManager();

private:
	/// The list of all the particle groups
	Particles m_vParticles;
};

#endif //__PARTICLEMANAGER_H__