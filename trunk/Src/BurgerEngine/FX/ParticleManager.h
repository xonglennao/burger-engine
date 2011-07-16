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
	Particles& GrabNewParticles(){return m_vNewParticles;}

	/// \brief Transfer the newly emitted particules to the main vector
	void TransferParticles();

	unsigned int GetParticleCount() const {return m_vParticles.size();}
private:
	/// The list of all the particle groups
	Particles m_vParticles;
	/// All particle emitted this frame
	Particles m_vNewParticles;
};

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
inline Particle& ParticleManager::RequestNewParticle()
{
	Particle oParticle;
	m_vNewParticles.push_back(oParticle);
	return m_vNewParticles.at(m_vNewParticles.size()-1);
}


#endif //__PARTICLEMANAGER_H__