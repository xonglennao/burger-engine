#include "BurgerEngine/fx/ParticleLifeEffector.h"
#include "BurgerEngine/fx/ParticleManager.h"
#include "BurgerEngine/fx/Particle.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleLifeEffector::ApplyEffect( float a_fTime, ParticleManager& a_rManager )
{
	//Init Life
	FOR_EACH_IT(Particles, a_rManager.GrabNewParticles(), it)
	{
		Particle& rParticle = (*it);
		float fLife = (float)rand()/(float)RAND_MAX * (m_fInitLifeMax - m_fInitLifeMin);
		fLife += m_fInitLifeMin;
		rParticle.fLife = fLife;
	}

	//UpdateLife
	Particles& rParticles = a_rManager.GrabParticles();
	Particles::iterator itUpdate = rParticles.begin();

	/// \todo Create a macro witha delete predicat
	// The update of life might go to the update core parameters in the particle group class
	// here we might consider only removing the particle if nedded
	while (itUpdate != rParticles.end())
	{
		Particle& rParticle = (*itUpdate);
		rParticle.fLife -= a_fTime;
		if (rParticle.fLife <0.0)
		{
			itUpdate = rParticles.erase(itUpdate);
		}
		else
		{
			++itUpdate; 
		}

	}

}