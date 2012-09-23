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

	const float4x4& GetViewMatrix() const;

private:
	/// \brief Calculate new Position from updated parameters
	void _InternalUpdate();

	// compute the position from basic inputs (keyboard, button etc.)
	void _UpdatePosition( float fDeltaTime );

	//compute the position from analog inputs (joystick...)
	void _UpdatePositionAnalog( float fDeltaTime );

	vec3 m_f3CenterPosition;
	float m_fRadius; 
};


#endif //__SPHERECAMERA_H__
