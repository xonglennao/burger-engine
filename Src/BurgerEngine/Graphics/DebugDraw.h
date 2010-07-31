/*************************************
*
*		BurgerEngine Project
*		
*		Created :	20/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __DEBUGDRAW_H__
#define __DEBUGDRAW_H__

#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/External/Osg/Vec3"

/// \class DebugDraw
/// \brief Use For quick display
class DebugDraw
{
public:

	/// \brief Draw a cube
	static void DrawCube(osg::Vec3f const& a_rf3Position, float a_fSize);

	/// \brief Draw Axis
	static void DrawAxis(osg::Vec3f const& a_rf3Position, float a_fSize);

	/// \brief Draw a grid
	static void DrawGrid(float a_fSize, unsigned int a_fSubdivision);


};


#endif //__DEBUGDRAW_H__