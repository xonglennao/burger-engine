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
#include "CommonGraphics.h"

class ImageTool
{
	public:
		//Load an Image, and return the image data + Width, Height and chanel
		static GLubyte* loadImage(const std::string& sName, unsigned int& iWidthOut,unsigned int& iHeightOut, unsigned int& iChannelOut);
};


#endif