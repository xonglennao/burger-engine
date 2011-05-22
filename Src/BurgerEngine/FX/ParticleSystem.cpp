#include "BurgerEngine/fx/ParticleSystem.h"
#include "BurgerEngine/fx/ParticleGroup.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleSystem::Play()
{
	m_bIsRunning = true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleSystem::Pause()
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleSystem::Stop()
{
	m_bIsRunning = false;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleSystem::Update()
{
	FOR_EACH_IT(ParticleGroups, m_vGroups, it)
	{
		(*it)->Update();
	}
}