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

#ifndef __CAMERAFPS_H__
#define __CAMERAFPS_H__

#include "BurgerEngine/Core/AbstractCamera.h"

/// \class CameraFps
/// \brief Interface class for every camera
class CameraFps: public AbstractCamera
{
public:
	/// \brief constructor
	CameraFps(){}

	/// \brief destructor
	~CameraFps(){}

	/// \brief Initialize Camera (set up callback)
	void Initialize();

	/// \brief Terminate Camera (free callback)
	void Terminate();

	/// \brief the main update fonction, for position etc...
	void Update( float fDeltaTime );

	void UpdateAngles( float a_fAddToAlpha, float a_fAddToPhi );

	/// \brief Set flags related to camera movement
	virtual void SetFlag( CameraFlagEnum eFlag, bool bValue);

private:

	/// \brief Grab direction the camera is pointing at
	vec3& _GrabDirection(){return m_f3Direction;}

	/// \brief Calculate new Position from updated parameters
	/// \todo bap: Need to calculate ourself the cross product.. can't the lib do it?
	void _InternalUpdate();

	/// Right position ?
	vec3 m_f3Right;

	/// The direction the camera in pointing at
	vec3 m_f3Direction;

	/// Moving speed into space
	float m_fMovingSpeed;

	/// Mouse speed (sensibility)
	float m_fMouseSpeed;

	/// These booleans are set to true when the corresponding key is pressed, in order to avoid keyboard repeat delay
	bool m_bForward;
	bool m_bBackward;
	bool m_bLeft;
	bool m_bRight;

};


#endif //__CAMERAFPS_H__
