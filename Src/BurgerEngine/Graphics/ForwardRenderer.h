/*************************************
*
*		BurgerEngine Project
*		
*		Created :	05/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __FORWARDRENDERER_H__
#define __FORWARDRENDERER_H__

/// \name	ForwardRenderer.h
/// \brief	The application of the window

#include "CommonGraphics.h"
#include <vector>

class FBO;

class ForwardRenderer
{
public:

	/// \brief default constructor
	ForwardRenderer();
	~ForwardRenderer();

	/// \brief strats rendering process
	void BeginRender();

	/// \brief
	bool Initialize();

	/// \brief
	void Terminate();

private:
	FBO * m_oFBOColorDepth;
	FBO * m_oFBOWriteAlpha;
	FBO * m_oFBOBlurH;
	FBO * m_oFBOBlurV;
	FBO * m_oFinalFBO;
	FBO * m_oPostProcessFBO0;
	FBO * m_oPostProcessFBO1;
};

#endif //__FORWARDRENDERER_H__
