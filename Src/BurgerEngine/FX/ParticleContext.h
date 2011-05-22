/*************************************
*
*		BurgerEngine Project
*		
*		Created :	01/05/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __PARTICLECONTEXT_H__
#define __PARTICLECONTEXT_H__

#include "BurgerEngine/fx/FXCommon.h"
#include <vector>

class ParticleSystem;

class ParticleContext
{
	typedef std::vector<ParticleSystem*> ParticleSystems;
public:

	/// \brief Update the context, so the particle
	void Update();

	/// \brief Set the system to be updated
	void RegisterFXInstance(ParticleSystem& a_rSystem);
	void UnregisterFXInstance(ParticleSystem& a_rSystem);


private:
	/// The collection of all system to be updated
	ParticleSystems m_oSystems;
};

#endif //__PARTICLECONTEXT_H__