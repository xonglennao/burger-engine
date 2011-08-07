#include "BurgerEngine/fx/ParticleEmitter.h"
#include "BurgerEngine/fx/ParticleManager.h"
#include <stdlib.h>
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleEmitter::Emit(float a_fTime, ParticleManager& a_rManager)
{
	/// Particle to emit this frame
	float fEmitCount = m_fEmissionFrequency * a_fTime;

	// Adding left over from last frame
	fEmitCount += m_fPreviousFrameParticle;

	// BURST: if there is a burst list to process, we check it

	// We Emit only if there is something to emit
	if (fEmitCount >= 1.0f)
	{
		unsigned int uToEmit = floor(fEmitCount);
		unsigned int uCountTotal = uToEmit + a_rManager.GetParticleCount();
		if (uCountTotal > m_uMaxCount)
		{
			// Recompute the number of particle to emit as many as we can
			uToEmit = m_uMaxCount - a_rManager.GetParticleCount();

			//Reseting the Count, In this case we do no store leftovers for next frame
			fEmitCount = float(uToEmit);
		}

		// If we follow the generator, then the position of the particle is NULL, and we will give 
		// the world matrix. If not, we need to give the current particleSystem location
		/// \todo Create a Shape that will allow to spawn at a special position
		vec3 f3PositionToSpawn(1.0);
		/*if (IsFollowingEmitter())
		{
			f3PositionToSpawn.SetNull();
		}
		else
		{
			f3PositionToSpawn = a_rRuntimeContext.GetCurrentPosition();
		}*/

			//As long as we have 1 particle to emit
		while(fEmitCount >= 1.0f )
		{
			std::cout<<"Emit!"<<std::endl;
			Particle& rParticle = a_rManager.RequestNewParticle();
			rParticle.f3Position = f3PositionToSpawn;
			rParticle.fMaxLife = 10.0f;

			
			float iX =  rand() % 5 - 2; 
			float iY = 70 + rand() % 2 - 1; 
			float iZ =  rand() % 5 -2; 
			rParticle.f3Velocity = vec3(iX,iY,iZ);
			rParticle.f3Acceleration = vec3(0.0);

			///Temp
			rParticle.f2Size = 0.5;
			rParticle.aUVs[0] = vec2(0.0,1.0);
			rParticle.aUVs[1] = vec2(0.0,0.0);
			rParticle.aUVs[2] = vec2(1.0,1.0);
			rParticle.aUVs[3] = vec2(1.0,0.0);
			/// \todo When a Particle pool will be ready, here how a particle should be set
			/// First you grab a new particle from the pool (an available one)
			/// Then you set the parameters
			/// Finally you stick a Ref of this particle into a vector of Newly emitted particle Ref
			/// This vector will be used for Effectors that Inits particle
			/// The actual method do this, but need a copy at the end of the ParticleSystemUpdate
			/*
			gnParticle& rParticle = a_rRuntimeContext.GrabNextParticle();

			rParticle.SetPosition(fPositionToSpawn);
			rParticle.SetLife(0.0f);

			// Keep the newly Emitted particle into a vector for "Init" effector
			gnVector<gnParticle*>& rNewParticles = a_rRuntimeContext.GrabNewlyEmittedParticle();
			rNewParticles.AddCopy(&rParticle)
			if (IsCollisionEnabled())
			{
				Insert the particle into the phys world
			}
			
			// Increment the particle count
			a_rRuntimeContext.IncrementParticleCount();*/

			fEmitCount -= 1.0f;
		}
	}
	
	//We store the amount of particle (less than one) for next frame
	m_fPreviousFrameParticle = fEmitCount;

}