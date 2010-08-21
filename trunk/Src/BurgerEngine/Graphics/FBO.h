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

#ifndef __FBO_H__
#define __FBO_H__

/// \name	FBO.h
/// \brief	OpenGL FrameBufferObjects

#include "CommonGraphics.h"

class FBO
{
public:
	enum FboEnum
	{
		E_FBO_CUBE = 0,
		E_FBO_2D,
	};


private:
		///Check the status of the FBO created
	bool checkFramebufferStatus();

	void generateID();
	void destroy();

	GLuint m_iTexId;
	GLuint m_iTexDepthId;
	GLuint m_iRenderId;
	GLuint m_iId;

	bool	m_bIsActivated;
	bool	m_bIsCubeMap;

	unsigned int m_iWidth;
	unsigned int m_iHeight;

	///Type : 0 = Color only , 1 = Depth only , 2 = Depth+Color
	///TODO : Enum 
	unsigned int m_iType;
	FboEnum m_eDefinitionType;

	GLuint m_eTextureType;

public:

	FBO(unsigned int iWidth, unsigned int iHeight, FboEnum eType);
	~FBO(){destroy();};

	void generateDepthOnly();
	void generateColorOnly(bool tex16f = false);
	void generate();

	//To activate the texture
	void activateTexture();
	void desactivateTexture();

	//To activate the depth texture
	void activateDepthTexture();
	void desactivateDepthTexture();

	//To activate the FBO (in order to write)
	void activate();
	void desactivate();
	//To choose a face for the cube 
	void activate(unsigned int iFace);

	inline const unsigned int& getWidth(){return m_iWidth;};
	inline const unsigned int& getHeight(){return m_iHeight;};
};

#endif //__FBO_H__
