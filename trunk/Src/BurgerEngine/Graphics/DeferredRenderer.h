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

#include "CommonGraphics.h"
#include "BurgerEngine/Graphics/EffectTechnique.h"
#include "BurgerEngine/Graphics/OmniLight.h"
#include "BurgerEngine/Graphics/SpotLight.h"
#include "BurgerEngine/Graphics/SpotShadow.h"

#include "BurgerEngine/External/Math/Vector.h"

#include <vector>

class SceneMesh;
class SceneGraph;
class OpenGLContext;

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
	void DrawFrustum( const vec3 * pPoints, const vec3& f3Pos );

	/// \brief draw Creates and stores 1 screen space quad per omni light
	void PrepareOmniLights( std::vector< OmniLight* > const& oOmniLights, AbstractCamera const& rCamera, Frustum const& oViewFrustum, float4x4 const& mModelView, float4x4 const& mModelViewProjection );
	/// \brief Displays previously created quads using 1 VBO
	void RenderOmniLights( std::vector< OmniLight::OmniLightQuad > vOmniLightQuads );	

	/// \brief draw Creates and stores 1 screen space quad per spot light
	void PrepareSpotLights( std::vector< SpotLight* > const& oSpotLights, AbstractCamera const& rCamera, Frustum const& oViewFrustum, float4x4 const& mModelView, float4x4 const& mModelViewProjection );
	bool ComputeOneSpotBoundingQuad( SpotLight* pLight, AbstractCamera const& rCamera, Frustum const& oViewFrustum, float4x4 const& mModelView, float4x4 const& mModelViewProjection, SpotLight::SpotLightQuad& oQuad );
	/// \brief Displays previously created quads using 1 VBO
	void RenderSpotLights( std::vector< SpotLight::SpotLightQuad > vSpotLightQuads, unsigned int iColorAndInverseRadiusHandle, unsigned int iViewSpacePosAndMultiplierHandle, unsigned int iViewSpaceDirHandle, unsigned int iCosInAndOutHandle );

	void PrepareAndRenderSpotShadows( const std::vector< SpotShadow* >& oSpotShadows, const AbstractCamera & rCamera, const Frustum& oViewFrustum, const float4x4& mModelView, const float4x4& mModelViewProjection );

	void RenderShadowMaps( const std::vector< SceneMesh* >& oSceneMeshes,SceneGraph& rSceneGraph, OpenGLContext& rRenderingContext );

	/// \brief Display 2D text on the screen
	void DisplayText( std::string const& sText, int iPosX, int iPosY );

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
	unsigned int m_iOmniLightShaderInvProjHandle;
	unsigned int m_iOmniLightShaderColorAndInverseRadiusHandle;
	unsigned int m_iOmniLightShaderViewSpacePosAndMultiplierHandle;

	Shader* m_pSpotLightShader;
	unsigned int m_iSpotLightShaderInvProjHandle;
	unsigned int m_iSpotLightShaderColorAndInverseRadiusHandle;
	unsigned int m_iSpotLightShaderViewSpacePosAndMultiplierHandle;
	unsigned int m_iSpotLightShaderViewSpaceDirHandle;
	unsigned int m_iSpotLightShaderCosInAndOutHandle;

	Shader* m_pSpotShadowShader;
	unsigned int m_iSpotShadowShaderInvProjHandle;
	unsigned int m_iSpotShadowShaderShadowMatrixHandle;
	unsigned int m_iSpotShadowShaderColorAndInverseRadiusHandle;
	unsigned int m_iSpotShadowShaderViewSpacePosAndMultiplierHandle;
	unsigned int m_iSpotShadowShaderViewSpaceDirHandle;
	unsigned int m_iSpotShadowShaderCosInAndOutHandle;

	std::vector< OmniLight::OmniLightQuad > m_vOmniLightQuads;
	std::vector< SpotLight::SpotLightQuad > m_vSpotLightQuads;
};

#endif //__DEFERREDRENDERER_H__
