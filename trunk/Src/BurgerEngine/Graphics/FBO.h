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

#include "BurgerEngine/Graphics/CommonGraphics.h"

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
	bool CheckFramebufferStatus();

	void GenerateID();
	void Destroy();

	GLuint m_iTexId[4];
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
	~FBO(){ Destroy(); };

	void GenerateDepthOnly();
	void GenerateColorOnly( GLint iInternalFormat = GL_RGBA8, GLint iFormat = GL_RGBA );
	void Generate( GLint iInternalFormat = GL_RGBA8, GLint iFormat = GL_RGBA );
	void GenerateFinalHDRBuffer( GLint iInternalFormat0 = GL_RGBA16F_ARB, GLint iFormat0 = GL_RGBA, GLint iInternalFormat1 = GL_R8, GLint iFormat1 = GL_RED );

	//To activate the texture
	void ActivateTexture();
	void DeactivateTexture();
	void ActivateTexture( unsigned int i );

	//To activate the depth texture
	void ActivateDepthTexture();
	void DeactivateDepthTexture();

	//To activate the FBO (in order to write)
	void Activate();
	void Deactivate();
	//To choose a face for the cube 
	void Activate(unsigned int iFace);

	inline const unsigned int& GetWidth(){return m_iWidth;};
	inline const unsigned int& GetHeight(){return m_iHeight;};
};

#endif //__FBO_H__
