#include "BurgerEngine/fx/ParticleVelocityEffector.h"
#include "BurgerEngine/fx/ParticleManager.h"
#include "BurgerEngine/fx/Particle.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleVelocityEffector::ApplyEffect( float a_fTime, ParticleManager& a_rManager )
{
	//Add Init velocity
	vec3 fDiff = m_f3InitMaxVelocity - m_f3InitMinVelocity;
	FOR_EACH_IT(Particles, a_rManager.GrabNewParticles(), it)
	{
		Particle& rParticle = (*it);
		float iX =  (float)rand()/(float)RAND_MAX * (fDiff.x) + m_f3InitMinVelocity.x; 
		float iY = (float)rand()/(float)RAND_MAX * (fDiff.y) + m_f3InitMinVelocity.y; 
		float iZ =  (float)rand()/(float)RAND_MAX * (fDiff.z) + m_f3InitMinVelocity.z; 
		rParticle.f3Velocity = vec3(iX,iY,iZ);
	}
}
