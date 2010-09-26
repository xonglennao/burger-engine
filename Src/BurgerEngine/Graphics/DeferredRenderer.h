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

#ifndef __DEFERREDRENDERER_H__
#define __DEFERREDRENDERER_H__

/// \name	DeferredRenderer.h
/// \brief	The application of the window

#include "CommonGraphics.h"
#include "BurgerEngine/Graphics/EffectTechnique.h"

#include <vector>

class FBO;
class Shader;
class PixelPerfectGLFont;
class Timer;

class DeferredRenderer
{
public:

	/// \brief default constructor
	DeferredRenderer();
	~DeferredRenderer();

	void SetDebugFlag( int i){ m_iDebugFlag = i; }

	/// \brief strats rendering process
	void Render();

private:
	/// \brief Display a full screen quad
	/// this is temporary, we need a fullscreenquad class using VBO
	void DrawScreenSpaceQuad( int iWindowWidth, int iWindowHeight );

	/// \brief Display 2D text on the screen
	void DisplayText( const std::string& sText, int iPosX, int iPosY );

private:
	FBO* m_oGBuffer;
	FBO* m_oLightBuffer;

	int m_iDebugFlag;
	PixelPerfectGLFont* m_oFont;

	Timer* m_oTimer;

	Shader* m_pOmniLightShader;

	//constants for lights (might be temporary...)
	GLfloat * m_pLightZeros;
	GLfloat * m_pLightDir;
};

#endif //__DEFERREDRENDERER_H__
