#include "BurgerEngine/fx/ParticleContext.h"
#include "BurgerEngine/fx/ParticleSystem.h"

#include <algorithm>
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleContext::Update()
{
	FOR_EACH_IT(ParticleSystems, m_oSystems, it)
	{
		//Nope! only the registered
		ParticleSystem& rSystem = (**it);
		if (rSystem.IsRunning())
		{
			rSystem.Update();
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleContext::RegisterFXInstance(ParticleSystem& a_rSystem)
{
	m_oSystems.push_back(&a_rSystem);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleContext::UnregisterFXInstance(ParticleSystem& a_rSystem)
{
	ParticleSystems::iterator it = std::find(m_oSystems.begin(),
		m_oSystems.end(),
		&a_rSystem);
	if (it != m_oSystems.end())
	{
		m_oSystems.erase(it);
	}

}
