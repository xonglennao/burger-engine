#include "imageTool.h"

#include "BurgerEngine/External/IL/il.h"

#include <iostream>

#undef _UNICODE

GLubyte* ImageTool::loadImage(const std::string& sName, unsigned int& iWidthOut,unsigned int& iHeightOut, unsigned int& iChannelOut)
{
	std::cout<<"LOADING : " <<sName<<std::endl;
	/// we are using DevIL just as a img loader, just for the data, we'll handle the rest
	ILuint ImageName;
	
	///Create the name for the image
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	///Load the image and return a Bool whether it true or false
	if (!ilLoadImage(sName.c_str()))
	{
		//TODO: Better exit handling
		std::cout<<"ERROR : Couldn't load the image " <<sName<<std::endl;
		return NULL;
	}

	///Save the datas
	iWidthOut = ilGetInteger(IL_IMAGE_WIDTH);
	iHeightOut = ilGetInteger(IL_IMAGE_HEIGHT);
	iChannelOut = ilGetInteger(IL_IMAGE_BPP); // RGB? RGBA?

	if(iChannelOut==4)
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	ILubyte *data = ilGetData();
	//We need to retrieve the data out of the devIL Class
	GLubyte* img = new GLubyte[(size_t)(iWidthOut) * (size_t)(iHeightOut) * (size_t)(iChannelOut)];
	memcpy(img, data, (size_t)(iWidthOut) * (size_t)(iHeightOut) * (size_t)(iChannelOut));

    // Suppression de la texture
    ilBindImage(0);
    ilDeleteImages(1, &ImageName);
	
	return img;

}

GLubyte* ImageTool::load3DFrom2DImage( const std::string& sName, unsigned int iWidth,unsigned int iHeight, unsigned int iDepth, unsigned int& iChannelOut )
{
	std::cout<<"LOADING : " <<sName<<std::endl;
	/// we are using DevIL just as a img loader, just for the data, we'll handle the rest
	ILuint ImageName;
	
	///Create the name for the image
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);

	///Load the image and return a Bool whether it true or false
	if (!ilLoadImage(sName.c_str()))
	{
		//TODO: Better exit handling
		std::cout<<"ERROR : Couldn't load the image " <<sName<<std::endl;
		return NULL;
	}

	///Save the datas
	unsigned int i2DWidth = ilGetInteger(IL_IMAGE_WIDTH);
	unsigned int i2DHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	iChannelOut = ilGetInteger(IL_IMAGE_BPP); // RGB? RGBA?

	if(iChannelOut==4)
	{
		//TODO: Better exit handling
		std::cout<<"ERROR : 4 channel 3D textures not supported: " <<sName<<std::endl;
		return NULL;
	}

	ILubyte *data = ilGetData();
	//We need to retrieve the data out of the devIL Class
	unsigned int iDataSize = (size_t)(iWidth) * (size_t)(iHeight) * (size_t)(iDepth) * (size_t)(iChannelOut);
	GLubyte* img = new GLubyte[ iDataSize ];
	unsigned int l = 0;
	for( unsigned int i = 0; i < iHeight; ++i )
	{
		for( unsigned int j = 0; j < iDepth; ++j )
		{
			for( unsigned int k = 0; k < iWidth; ++k )
			{
				unsigned int iIndex = (i * iWidth + j * i2DWidth + k) * 3;
				img[ l * 3 ] = data[ iIndex ];
				img[ l * 3 + 1 ] = data[ iIndex + 1 ];
				img[ l * 3 + 2 ] = data[ iIndex + 2 ];
				++l;
			}
		}
	}
	//memcpy(img, data, (size_t)(iWidthOut) * (size_t)(iHeightOut) * (size_t)(iChannelOut));

    // Suppression de la texture
    ilBindImage(0);
    ilDeleteImages(1, &ImageName);
	
	return img;

}