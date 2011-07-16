#include "BurgerEngine/fx/ParticleAccelerationEffector.h"
#include "BurgerEngine/fx/ParticleManager.h"
#include "BurgerEngine/fx/Particle.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleAccelerationEffector::ApplyEffect( float a_fTime, ParticleManager& a_rManager )
{
	//Init Acceleration (gravity)
	FOR_EACH_IT(Particles, a_rManager.GrabNewParticles(), it)
	{
		Particle& rParticle = (*it);
		rParticle.f3Acceleration = m_f3InitAcceleration;
	}

}
