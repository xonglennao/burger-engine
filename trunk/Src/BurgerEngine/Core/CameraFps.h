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
	void Update();

	/// \brief What to do when the user press the down key
	bool OnDownKey(unsigned char a_cKey);

	/// \brief When the screen is Resize, when want the new size to be stored
	bool OnResize(unsigned int a_uWidth, unsigned int a_uHeight);

	/// \brief What action to do when the user move the mouse
	bool OnMouseMoved(unsigned int a_uX, unsigned int a_uY);

	/// \brief Set flags relative to camera movement
	virtual void SetFlag( CameraFlagEnum eFlag, bool bValue);

private:

	/// \brief Grab direction the camera is pointing at
	osg::Vec3f& _GrabDirection(){return m_f3Direction;}

	/// \brief Calculate new Position from updated parameters
	/// \todo bap: Need to calculate ourself the cross product.. can't the lib do it?
	void _VectorsFromAngles();

	/// Right position ?
	osg::Vec3f m_f3Right;

	/// The direction the camera in pointing at
	osg::Vec3f m_f3Direction;

	/// Size of the window
	osg::Vec2f m_f2WindowSize;

	/// Moving speed into space
	float m_fMovingSpeed;

	/// Mouse speed (sensibility)
	float m_fMouseSpeed;

	/// Math variables for camera
	/// \todo bap: add detail
	float m_fAlpha;
	float m_fPhi;	

	/// These booleans are set to true when the correspondign key is pressed, in order to avoid repeat delay
	bool m_bForward;
	bool m_bBackward;
	bool m_bLeft;
	bool m_bRight;

};


#endif //__CAMERAFPS_H__
