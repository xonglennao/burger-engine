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

typedef std::vector<Particle> Particles; 

/// \brief Manager the memory for one layer of effect
/// It equal to one batch
class ParticleManager
{
	
public:
	/// \brief constructor
	ParticleManager()
	  {}
	~ParticleManager();

	/// \brief Init the Vector
	void Initialize(unsigned int a_uMaxCount);

	/// \brief Get a new Particle
	Particle& RequestNewParticle();

	/// \ brief Get the whole particle list
	Particles& GrabParticles(){return m_vParticles;}

	unsigned int GetParticleCount() const {return m_vParticles.size();}
private:
	/// The list of all the particle groups
	Particles m_vParticles;
};

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
inline Particle& ParticleManager::RequestNewParticle()
{
	Particle oParticle;
	m_vParticles.push_back(oParticle);
	return m_vParticles.at(m_vParticles.size()-1);
}


#endif //__PARTICLEMANAGER_H__