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


#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "BurgerEngine/fx/FXCommon.h"
#include <vector>

class ParticleGroup;

class ParticleSystem
{
	typedef std::vector<ParticleGroup*> ParticleGroups; 
public:
	/// \brief constructor
	ParticleSystem():
		m_bIsRunning(false)
		{}

	~ParticleSystem(){}

	/// \brief Play the Particle System
	void Play();

	void Pause();

	void Stop();

	/// \brief Update all the layers
	void Update();

	/// \brief Is ruuning
	bool IsRunning(){return m_bIsRunning;}


private:
	/// The list of all the particle groups
	ParticleGroups m_vGroups;
	
	/// Is the FX Running?
	bool m_bIsRunning;
};

#endif //__PARTICLESYSTEM_H__