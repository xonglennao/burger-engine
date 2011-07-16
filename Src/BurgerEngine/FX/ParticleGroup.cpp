#include "BurgerEngine/fx/ParticleGroup.h"
#include "BurgerEngine/fx/ParticleManager.h"
#include "BurgerEngine/fx/ParticleEffector.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/TimeContext.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::Initialize()
{
	m_pManager = new ParticleManager();
	m_pManager->Initialize(50);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::Terminate()
{
	delete m_pManager;
	m_pManager = NULL;
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::Play()
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::Pause()
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::Stop()
{

}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::Update()
{
	//Retrieve Time
	float fTime = Engine::GetInstance().GetTimeContext().GetScaledTime();

	// Has the emmitter completed one cycle
	float fLife = m_oEmitter.GetCurrentLife();
	float fMaxLife = m_oEmitter.GetMaxLife();
	if (fLife > fMaxLife)
	{
		// The period is over, we increase by one the number of loop
		unsigned int uMaxLoopCount = m_oEmitter.GetMaxLoopCount();
		unsigned int uLoopCount = m_oEmitter.GetLoopCount() + 1;

		// We have not yet reached the max count, or the Emitter has an infinite loop
		if (uMaxLoopCount == 0 || uLoopCount < uMaxLoopCount)
		{
			m_oEmitter.SetCurrentLife(0.0f);
			m_oEmitter.SetLoopCount(uLoopCount);
		}
		else
		{
			//m_bIsAboutToStop = true;
		}
	}
	else
	{
		// Update Life of Emitter
		m_oEmitter.SetCurrentLife(fLife + fTime);
	}

	if (m_bIsAboutToStop)
	{
		// Check how many particle are left into the manager
		// If none then we can stop it
	}
	else
	{
		//Emission
		m_oEmitter.Emit(fTime, *m_pManager);
	}

	//Update
	FOR_EACH_IT(ParticleEffectors, m_vEffector, it)
	{
		ParticleEffector& rEffector = (**it);
		rEffector.ApplyEffect(fTime, *m_pManager);
	}
	

	m_pManager->TransferParticles();
	//Update speed
	_UpdateCoreParameters(fTime);

	//Sort
	_Sort();

	//Send info to Gfx
	//gnGfxSceneContext& rGfxContext = a_rScene.GrabContext<gnGfxSceneContext>();
	//DrawParticle(rGfxContext);
	
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::_UpdateCoreParameters(float a_fTime)
{
	FOR_EACH_IT(Particles, m_pManager->GrabParticles(), it)
	{
		Particle& rParticle = (*it);

		// Compute new velocity
		rParticle.f3Velocity += rParticle.f3Acceleration * a_fTime;

		// Compute New Position
		rParticle.f3Position += rParticle.f3Velocity * a_fTime;
	}
	if (m_pManager->GrabParticles().size()>0)
	{
		std::cout<<"Y : "<<m_pManager->GrabParticles()[m_pManager->GrabParticles().size()-1].f3Position.y<<std::endl;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::_Sort()
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleGroup::_DrawParticle()
{

}

