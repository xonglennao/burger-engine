#include "BurgerEngine/fx/ParticleAccelerationEffector.h"
#include "BurgerEngine/fx/ParticleManager.h"
#include "BurgerEngine/fx/Particle.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleAccelerationEffector::ApplyEffect( float a_fTime, ParticleManager& a_rManager )
{
	//Init Acceleration (gravity)
	vec3 fDiff = m_f3InitMaxAcceleration - m_f3InitMinAcceleration;
	FOR_EACH_IT(Particles, a_rManager.GrabNewParticles(), it)
	{
		Particle& rParticle = (*it);
		float iX =  (float)rand()/(float)RAND_MAX * (fDiff.x) + m_f3InitMinAcceleration.x; 
		float iY = (float)rand()/(float)RAND_MAX * (fDiff.y) + m_f3InitMinAcceleration.y; 
		float iZ =  (float)rand()/(float)RAND_MAX * (fDiff.z) + m_f3InitMinAcceleration.z; 
		rParticle.f3Acceleration = vec3(iX,iY,iZ);
	}

}
