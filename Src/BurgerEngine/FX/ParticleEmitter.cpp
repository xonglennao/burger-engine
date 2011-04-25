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
	/// \brief Constructor
	ParticleEmitter(){}

	/// \brief Destructor
	~ParticleEmitter();

	/// \brief 

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

#endif //__PARTICLEEMITTER_H__