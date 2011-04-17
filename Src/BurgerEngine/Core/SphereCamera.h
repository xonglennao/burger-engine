/*************************************
*
*		BurgerEngine Project
*		
*		Created :	17/04/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __SPHERECAMERA_H__
#define __SPHERECAMERA_H__

#include "BurgerEngine/Core/AbstractCamera.h"

/// \class SphereCamera
class SphereCamera: public AbstractCamera
{
public:
	/// \brief constructor
	SphereCamera( float fRadius, float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& fSpeed  );

	/// \brief destructor
	~SphereCamera(){}

	/// \brief the main update fonction, for position etc...
	void Update( float fDeltaTime );

	void LookAt();
	float4x4 const GetViewMatrix() const;

private:
	/// \brief Calculate new Position from updated parameters
	void _InternalUpdate();
	vec3 m_f3PositionOnSphere;
	float m_fRadius; 
};


#endif //__SPHERECAMERA_H__
