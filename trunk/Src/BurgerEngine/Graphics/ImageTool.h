/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/08/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __IMAGETOOL_H__
#define __IMAGETOOL_H__

#include <string>
#include "BurgerEngine/Graphics/CommonGraphics.h"

class ImageTool
{
	public:
		//Load an Image, and return the image data + Width, Height and chanel
		static GLubyte* loadImage(const std::string& sName, unsigned int& iWidthOut,unsigned int& iHeightOut, unsigned int& iChannelOut);

		//Load a 2D Image and convert the data in order to use it as a 3D texture
		static GLubyte* load3DFrom2DImage( const std::string& sName, unsigned int iWidth,unsigned int iHeight, unsigned int iDepth, unsigned int& iChannelOut );
};


#endif