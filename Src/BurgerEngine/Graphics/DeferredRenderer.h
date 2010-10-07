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
#include "BurgerEngine/Graphics/SceneLight.h"

class FBO;
class Shader;
class PixelPerfectGLFont;
class Timer;
class Frustum;
struct vec3;
class AbstractCamera;

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
	/// this is temporary, we need a to use VBO
	void DrawFullScreenQuad( int iWindowWidth, int iWindowHeight );

	/// \draw a quad on the screen
	/// this is only for debug purpose
	void DrawScreenSpaceQuad( int iWindowWidth, int iWindowHeight, vec3 vData );

	/// \brief draw Creates and stores 1 screen space quad per omni light
	void DeferredRenderer::PrepareOmniLights( const std::vector< SceneLight* >& oOmniLights, const AbstractCamera & rCamera, const Frustum& oViewFrustum, const float4x4& mModelView, const float4x4& mModelViewProjection );
	/// \brief Displays previously created quads using 1 VBO
	void RenderOmniLights( std::vector< SceneLight::OmniLightQuad > vOmniLightQuads );	

	/// \brief Display 2D text on the screen
	void DisplayText( const std::string& sText, int iPosX, int iPosY );

private:
	FBO* m_oGBuffer;
	FBO* m_oLightBuffer;

	int m_iDebugFlag;

	PixelPerfectGLFont* m_oFont;

	Timer*	m_oTimer;
	float	m_fFrameCount;
	float	m_fMaxFrame;
	float	m_fFrameTime;

	Shader* m_pOmniLightShader;
	unsigned int m_iOmniLightShaderInvMVPHandle;
	unsigned int m_iOmniLightShaderColorAndInverseRadiusHandle;
	unsigned int m_iOmniLightShaderViewSpacePosAndMultiplierHandle;

	std::vector< SceneLight::OmniLightQuad > m_vOmniLightQuads;
};

#endif //__DEFERREDRENDERER_H__
