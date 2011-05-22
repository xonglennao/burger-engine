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


#ifndef __PARTICLE_H__
#define __PARTICLE_H__


#include "BurgerEngine/External/Math/Vector.h"

/// \brief A single particle 
struct  Particle
{
	/// Main Color
	vec4 f4Color;
	/// Runtime Position
	vec3 f3Position;
	/// Velocity (speed and direction)
	vec3 f3Velocity;
	/// Size (XY);
	vec2 f2Size;
	/// The 4 UV coordinates;
	vec2 aUVs[4];
	/// The runtime rotation
	float fRotation;
	/// The speed of the rotation
	float fRotationRate;
	/// The current life
	float fLife;
	/// The life it has to reach before dying
	float fMaxLife;
	/// The sorting value in case of Z sorting 
	float fZValue;
	/// Is the particle alive or note (in case of pool management)
	bool bIsAlive;

};

#endif //__PARTICLE_H__
