/*************************************
*
*		BurgerEngine Project
*		
*		Created :	22/05/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/


/// \brief A Particle group hold One manager, One emitter and several effector

#ifndef __PARTICLEGROUP_H__
#define __PARTICLEGROUP_H__

#include "BurgerEngine/fx/FXCommon.h"
#include "BurgerEngine/fx/ParticleEmitter.h"
#include <vector>

class ParticleGroup
{
	//typedef std::vector<ParticleGroup*> ParticleGroups; 
public:
	/// \brief constructor
	ParticleGroup(){}
	~ParticleGroup(){}

	void Initialize();

	/// \brief Play the Particle System
	void Play();

	void Pause();

	void Stop();

	/// \brief Update all the layers
	void Update();

private:
	/// The emitter
	ParticleEmitter m_oEmitter;

	/// The manager
	/// \todo we might use one pool for all particle
	ParticleManager* m_pManager;

};

#endif //__PARTICLEGROUP_H__