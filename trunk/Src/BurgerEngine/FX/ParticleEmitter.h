/*************************************
*
*		BurgerEngine Project
*		
*		Created :	25/04/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/


#ifndef __PARTICLEEMITTER_H__
#define __PARTICLEEMITTER_H__

#include "BurgerEngine/fx/FXCommon.h"

enum EmissionPolicy
{
	WAIT,
	ERASE_OLDER
};

/// \Brief Where will be emitted the particle on shape (On edge, inside, etc...)
enum EmissionPosition
{
	VERTEX,
	EDGE,
	SURFACE,
	VOLUME
};

/// \todo BurstList
class ParticleEmitter
{
	friend class ParticleComponent;
public:
	/// \brief Constructor
	ParticleEmitter():
		m_ePolicy(ERASE_OLDER),
		m_eEmissionPos(VERTEX),
		m_fPreviousFrameParticle(0.0f)
		{}

	/// \brief Destructor
	~ParticleEmitter(){}

	/// \brief Emit a certain amount of particle into the ParticleContext
	void Emit(float a_fTime, ParticleManager& a_rManager);

	/// \brief Get MaxLife
	float GetMaxLife() const {return m_fLife;}

	/// \brief Get and set MaxLife
	float GetCurrentLife() const {return m_fCurrentLife;}
	void SetCurrentLife(float a_fLife){m_fCurrentLife = a_fLife;}

	/// \breif Get the count of emitting cycle
	unsigned int GetMaxLoopCount() const {return m_uLoopCount;}
	unsigned int GetLoopCount() const {return m_uCurrentLoop;}
	void SetLoopCount(unsigned int a_uLoopCount) {m_uCurrentLoop  = a_uLoopCount;}

private:

	/// \todo the following will be LODable in the future
	/// Max count particle to Emit
	unsigned int m_uMaxCount;

	// How many times the emitter will loop before stopping, if 0 loop is infinite;
	unsigned int m_uLoopCount;

	/// The runtime loop count
	unsigned int m_uCurrentLoop;

	/// The emitter life periode, can be reapeated, or can be infinite
	float m_fLife;
	///  The current life 
	float m_fCurrentLife;

	/// frequency (Emission Rate) in second
	float m_fEmissionFrequency;

	/// Do the particle follows the emitter
	bool m_bFollowEmitter;

	/// Emission Policy
	EmissionPolicy m_ePolicy;
	/// Emmision Position
	EmissionPosition m_eEmissionPos;

	/// The number of particle to be emitted next time around
	/// if we emit less than 1 particle per frame, we need to store a float until we reach 1
	float m_fPreviousFrameParticle;
};

#endif //__PARTICLEEMITTER_