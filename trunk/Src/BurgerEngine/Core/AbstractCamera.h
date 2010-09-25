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

#ifndef __ABSTRACTCAMERA_H__
#define __ABSTRACTCAMERA_H__

#include "BurgerEngine/External/Math/Vector.h"

/// \class AbstractCamera
/// \brief Interface class for every camera
class AbstractCamera
{
public:
	enum CameraFlagEnum
	{
		 E_CAMERA_FORWARD
		,E_CAMERA_BACKWARD
		,E_CAMERA_LEFT
		,E_CAMERA_RIGHT
	};

public:
	/// \brief constructor
	AbstractCamera();

	/// \brief destructor
	virtual ~AbstractCamera(){}

	/// \brief the main update function, for position etc...
	virtual void Update() = 0;

	/// \brief Initialize Camera (set up callback)
	virtual void Initialize( const unsigned int a_iWindowWidth, const unsigned int a_iWindowHeight ) = 0;

	/// \brief Terminate Camera (free callback)
	virtual void Terminate() = 0;

	/// \brief Set flags related to camera movement
	virtual void SetFlag( CameraFlagEnum eFlag, bool bValue){};

	vec3 const& GetPos() const	{return m_f3Pos;}
	vec3 const& GetAim() const {return m_f3Aim;}
	vec3 const& GetUp() const {return m_f3Up;}

	void LookAt();

protected:
	vec3& _GrabPos(){return m_f3Pos;}
	vec3& _GrabAim(){return m_f3Aim;}
	vec3& _GrabUp(){return m_f3Up;}

private:

	/// The position Vector
	/// \todo On next implementation, we should use a full matrix instead of separate vector
	vec3 m_f3Pos;
	vec3 m_f3Aim;
	vec3 m_f3Up;

};


#endif //__ABSTRACTCAMERA_H__