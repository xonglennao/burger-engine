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

class ParticleRenderer;
class Material;

typedef std::vector<ParticleEffector*> ParticleEffectors;

class ParticleGroup
{
	//typedef std::vector<ParticleGroup*> ParticleGroups; 
public:
	/// \brief constructor
	ParticleGroup():
		m_bIsAboutToStop(false),
		m_pMaterial(NULL)
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

	/// \brief Add an effector
	void AddEffector(ParticleEffector& a_rEffector){m_vEffector.push_back(&a_rEffector);}

	/// \brief SetMaterial
	void SetMaterial(Material* a_pMaterial){m_pMaterial = a_pMaterial;}
private:
	/// \brief Update position 
	void _UpdateCoreParameters(float a_fTime);

	/// \brief Sort particle depeding on the 
	void _Sort();

	/// \brief Send particle to the GFX
	void _DrawParticle(ParticleRenderer& a_rRenderer);
private:
	/// The emitter
	ParticleEmitter m_oEmitter;

	/// The manager
	/// \todo we might use one pool for all particle
	ParticleManager* m_pManager;

	/// Is this Group is about to stop??
	bool m_bIsAboutToStop;

	/// the collection of effector
	ParticleEffectors m_vEffector;

	/// One material per group
	Material* m_pMaterial;
};

#endif //__PARTICLEGROUP_H__