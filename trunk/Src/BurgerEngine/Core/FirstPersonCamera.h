/*************************************
*
*		BurgerEngine Project
*		
*		Created :	16/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __FIRSTPERSONCAMERA_H__
#define __FIRSTPERSONCAMERA_H__

#include "BurgerEngine/Core/AbstractCamera.h"

/// \class FirstPersonCamera
class FirstPersonCamera: public AbstractCamera
{
public:
	/// \brief constructor
	FirstPersonCamera( float fFOV, const vec3& f3Pos, const vec2& f2Rotation, const vec4& f4DofParams, const vec2& fSpeed  );

	/// \brief destructor
	~FirstPersonCamera(){}

	const float4x4& GetViewMatrix() const;

	void SetPosition(vec3 const& f3Position) { m_f3Pos  = f3Position; };

private:
	/// \brief Calculate new Position from updated parameters
	void _InternalUpdate();


	// compute the position from basic inputs (keyboard, button etc.)
	void _UpdatePosition( float fDeltaTime );

	//compute the position from analog inputs (joystick...)
	void _UpdatePositionAnalog( float fDeltaTime );

	/// The direction the camera in pointing at
private:
	vec3 m_f3Direction;
};


#endif //__FIRSTPERSONCAMERA_H__
