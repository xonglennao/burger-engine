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
	ParticleGroup():
		m_bIsAboutToStop(false)
		{}
	~ParticleGroup(){}

	void Initialize();
	void Terminate();

	/// \brief Play the Particle System
	void Play();

	void Pause();

	void Stop();

	/// \brief Update all the layers
	void Update();

	/// \brief GrabEmitter
	ParticleEmitter& GrabEmitter(){return m_oEmitter;}

private:
	/// \brief Update position 
	void _UpdateCoreParameters(float a_fTime);

	/// \brief Sort particle depeding on the 
	void _Sort();

	/// \brief Send particle to the GFX
	void _DrawParticle();
private:
	/// The emitter
	ParticleEmitter m_oEmitter;

	/// The manager
	/// \todo we might use one pool for all particle
	ParticleManager* m_pManager;

	/// Is this Group is about to stop??
	bool m_bIsAboutToStop;
};

#endif //__PARTICLEGROUP_H__