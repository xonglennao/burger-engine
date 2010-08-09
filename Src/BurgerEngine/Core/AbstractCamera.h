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

#include "BurgerEngine/External/Osg/Vec3"

/// \class AbstractCamera
/// \brief Interface class for every camera
class AbstractCamera
{
public:
	/// \brief constructor
	AbstractCamera();

	/// \brief destructor
	virtual ~AbstractCamera(){}

	/// \brief the main update function, for position etc...
	virtual void Update() = 0;

	/// \brief Initialize Camera (set up callback)
	virtual void Initialize() = 0;

	/// \brief Terminate Camera (free callback)
	virtual void Terminate() = 0;

	osg::Vec3 const& GetPos() const	{return m_f3Pos;}
	osg::Vec3 const& GetAim() const {return m_f3Aim;}
	osg::Vec3 const& GetUp() const {return m_f3Up;}

protected:
	osg::Vec3& _GrabPos(){return m_f3Pos;}
	osg::Vec3& _GrabAim(){return m_f3Aim;}
	osg::Vec3& _GrabUp(){return m_f3Up;}

private:

	/// The position Vector
	/// \todo On next implementation, we should use a full matrix instead of separate vector
	osg::Vec3 m_f3Pos;
	osg::Vec3 m_f3Aim;
	osg::Vec3 m_f3Up;

};


#endif //__ABSTRACTCAMERA_H__