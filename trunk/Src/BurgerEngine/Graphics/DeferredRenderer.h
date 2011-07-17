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

#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/Graphics/EffectTechnique.h"
#include "BurgerEngine/Graphics/OmniLight.h"
#include "BurgerEngine/Graphics/SpotLight.h"
#include "BurgerEngine/Graphics/SpotShadow.h"
#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/ParticleRenderer.h"

#include "BurgerEngine/External/Math/Vector.h"

#include <vector>

class SceneGraph;
class OpenGLContext;
class Texture3D;

class DeferredRenderer
{
public:

	struct BackToFrontComp
	{
		bool operator() ( SceneMesh * i , SceneMesh * j ) { return ( i->GetViewZ() < j->GetViewZ() ); }
	};

	struct FrontToBackComp
	{
		bool operator() ( SceneMesh * i , SceneMesh * j ) { return ( i->GetViewZ() > j->GetViewZ() ); }
	};
	
	/// \brief default constructor
	DeferredRenderer();
	~DeferredRenderer();

	void SetDebugFlag( int i ){ m_iDebugFlag = i; }
	void SwitchShowDebugMenu(){ m_bShowDebugMenu = !m_bShowDebugMenu; }
	bool GetShowDebugMenu(){ return m_bShowDebugMenu; }

	void SetPostProcessParameters( float fToneMappingKey, float fGlowMultiplier, float fBrightPassThreshold, float fBrightPassOffset, float fAdaptationBaseTime, const char* pColorLUT );

	/// \brief strats rendering process
	void Render();

private:

	/// \brief Loads engine shaders such as lighting, shadow, blur etc.
	void LoadEngineShaders();

	void CreateFBO();

	/// \brief Generate a VBO for the full screen quad
	void GenFullScreenQuad();
	/// \brief Display a full screen quad
	void DrawFullScreenQuad( int iWindowWidth, int iWindowHeight, bool bCCW = true );

	/// \draw a quad on the screen
	/// this is only for debug purpose
	void DrawScreenSpaceQuad( int iWindowWidth, int iWindowHeight, vec3 vData );
	void DrawFrustum( const vec3 * pPoints, const vec3& f3Pos );
	void DrawCube( const vec3 * pPoints );

	/// \brief Creates and stores 1 screen space quad per directional light
	void PrepareDirectionalLights( std::vector< SceneLight* > const& oSceneLights, AbstractCamera const& rCamera, float4x4 const& mView );
	/// \brief Displays 1 full screen quad per Directional Light
	void RenderDirectionalLights( std::vector< SceneLight::SceneLightQuad > vDirectionalLightQuads );
	
	/// \brief Creates and stores 1 screen space quad per omni light
	void PrepareOmniLights( std::vector< OmniLight* > const& oOmniLights, AbstractCamera const& rCamera, float4x4 const& mView, float4x4 const& mViewProjection );
	/// \brief Displays previously created quads using 1 VBO
	void RenderOmniLights( std::vector< OmniLight::OmniLightQuad > vOmniLightQuads );	

	/// \brief Creates and stores 1 screen space quad per spot light
	void PrepareSpotLights( std::vector< SpotLight* > const& oSpotLights, AbstractCamera const& rCamera, float4x4 const& mView, float4x4 const& mViewProjection );
	void ComputeOneSpotBoundingQuad( SpotLight* pLight, AbstractCamera const& rCamera, float4x4 const& mView, float4x4 const& mViewProjection, SpotLight::SpotLightQuad& oQuad );
	/// \brief Displays previously created quads using 1 VBO
	void RenderSpotLights( std::vector< SpotLight::SpotLightQuad > vSpotLightQuads, unsigned int iColorAndInverseRadiusHandle, unsigned int iViewSpacePosAndMultiplierHandle, unsigned int iViewSpaceDirHandle, unsigned int iCosInAndOutHandle );

	void PrepareAndRenderSpotShadows( const std::vector< SpotShadow* >& oSpotShadows, const AbstractCamera & rCamera, const float4x4& mView, const float4x4& mViewProjection );

	void RenderShadowMaps( const std::vector< SceneMesh* >& oSceneMeshes, const std::vector< SpotShadow* >& oSpotShadows, OpenGLContext& a_rDriverRenderingContext );

	void ComputeAvgLum();

	/// \brief Display 2D text on the screen
	void DisplayText( std::string const& sText, int iPosX, int iPosY, PixelPerfectGLFont* pFont );

	void DisplayDebugMenu();

	void DebugRender( const std::vector< SceneMesh* >& oSceneMeshes,  const std::vector< SceneMesh* >& oTransparentSceneMeshes, const std::vector< SpotShadow* >& oSpotShadows, const std::vector< SpotLight* >& oSpotLights );

	/// \brief Do frustum culling test on meshes and lights
	void GetVisibleObjects( RenderingContext& rRenderContext, const Frustum& oViewFrustum, const float4x4& mView, std::vector< SceneMesh* >& oVisibleSceneMeshes, std::vector< SceneMesh* >& oVisibleTransparentSceneMeshes, std::vector< OmniLight* >& oVisibleOmniLights, std::vector< SpotLight* >& oVisibleSpotLights, std::vector< SpotShadow* >& oVisibleSpotShadows );

private:
	FBO* m_pGBuffer;
	FBO* m_pLightBuffer;
	FBO* m_pSpotShadowBlurBuffer;
	FBO* m_pHDRSceneBuffer;
	FBO* m_pLDRSceneBuffer;
	FBO* m_pLDRSceneBuffer2;

	//HDR & ToneMapping
	FBO* m_pDownSampledSceneBuffer;
	FBO* m_p64x64LumBuffer;
	FBO* m_p16x16LumBuffer;
	FBO* m_p4x4LumBuffer;
	FBO* m_p1x1LumBuffer;
	FBO* m_pCurrentAdaptationBuffer;
	FBO* m_pLastAdaptationBuffer;
	FBO* m_pBrightPass1Buffer;
	FBO* m_pBrightPass2Buffer;

	FBO* m_pDOFBlur1Buffer;
	FBO* m_pDOFBlur2Buffer;

	GLuint m_iFullScreenQuadBufferId;
	GLuint m_iFullScreenQuadBufferIdCW;
	
	//Debug variables
	int				m_iDebugFlag;
	int				m_iDebugRender;
	bool			m_bShowDebugMenu;
	int				m_iSkipCulling;
	int				m_iDebugBoundingBox;
	int				m_iDebugSpotLightFrustum;

	unsigned int	m_iObjectCount;
	unsigned int	m_iOmniCount;
	unsigned int	m_iSpotCount;
	unsigned int	m_iSpotShadowCount;
	unsigned int	m_iDirectionalCount;

	//Fonts used to display text on screen
	PixelPerfectGLFont* m_pFont;
	PixelPerfectGLFont* m_pFont2;

	//Profiler variables
	Timer*	m_oTimer;
	float	m_fFrameCount;
	float	m_fFrameTime;

	//Shader pointers and handles for uniform variables
	Shader*			m_pDirectionalLightShader;
	unsigned int	m_iDirectionalLightShaderInvProjHandle;
	unsigned int	m_iDirectionalLightShaderColor;
	unsigned int	m_iDirectionalLightShaderViewSpacePosAndMultiplierHandle;

	Shader*			m_pOmniLightShader;
	unsigned int	m_iOmniLightShaderInvProjHandle;
	unsigned int	m_iOmniLightShaderColorAndInverseRadiusHandle;
	unsigned int	m_iOmniLightShaderViewSpacePosAndMultiplierHandle;

	Shader*			m_pSpotLightShader;
	unsigned int	m_iSpotLightShaderInvProjHandle;
	unsigned int	m_iSpotLightShaderColorAndInverseRadiusHandle;
	unsigned int	m_iSpotLightShaderViewSpacePosAndMultiplierHandle;
	unsigned int	m_iSpotLightShaderViewSpaceDirHandle;
	unsigned int	m_iSpotLightShaderCosInAndOutHandle;

	Shader*			m_pSpotShadowShader;
	unsigned int	m_iSpotShadowShaderInvProjHandle;
	unsigned int	m_iSpotShadowShaderShadowMatrixHandle;
	unsigned int	m_iSpotShadowShaderColorAndInverseRadiusHandle;
	unsigned int	m_iSpotShadowShaderViewSpacePosAndMultiplierHandle;
	unsigned int	m_iSpotShadowShaderViewSpaceDirHandle;
	unsigned int	m_iSpotShadowShaderCosInAndOutHandle;

	Shader*			m_pExponentialShadowMapShader;
	
	Shader*			m_pBlur6Shader;
	unsigned int	m_iBlur6ShaderPixelSizeHandle;

	Shader*			m_pLogBlur10Shader;
	unsigned int	m_iLogBlur10ShaderPixelSizeHandle;

	Shader*			m_pDownSample4x4DOF;
	unsigned int	m_iDownSample4x4DOFPixelSizeHandle;

	Shader*			m_pBlur10Shader;
	unsigned int	m_iBlur10ShaderPixelSizeHandle;

	Shader*			m_pDownSample4x4Shader;
	unsigned int	m_iDownSampleShaderPixelSizeHandle;

	Shader*			m_pAvgLumInitShader;
	unsigned int	m_iAvgLumInitShaderPixelSizeHandle;

	Shader*			m_pAvgLumFinalShader;
	unsigned int	m_iAvgLumFinalShaderPixelSizeHandle;

	Shader*			m_pToneMappingShader;
	unsigned int	m_iToneMappingShaderKeyHandle;

	Shader*			m_pPostProcessShader;
	unsigned int	m_iPostProcessShaderGlowMultiplierHandle;

	Shader*			m_pLightAdaptationShader;
	unsigned int	m_iAdaptationShaderTimeHandle;

	Shader*			m_pBrightPassShader;
	unsigned int	m_iBrightPassShaderInvViewPortHandle;
	unsigned int	m_iBrightPassShaderThresholdOffsetKeyHandle;

	Shader*			m_pDOFShader;

	//Debug render shaders
	Shader*			m_pBasicTextureShader;
	Shader*			m_pDebugGlossShader;
	Shader*			m_pDebugSpecularShader;
	Shader*			m_pDebugDepthShader;

	//Post-process settings
	float m_fToneMappingKey;
	float m_fGlowMultiplier;
	float m_fBrightPassThreshold;
	float m_fBrightPassOffset;
	float m_fAdaptationBaseTime;

	std::vector< SceneLight::SceneLightQuad > m_vDirectionalLightQuads;
	std::vector< OmniLight::OmniLightQuad > m_vOmniLightQuads;
	std::vector< SpotLight::SpotLightQuad > m_vSpotLightQuads;

	Texture3D * m_pColorLUT;
};

#endif //__DEFERREDRENDERER_H__
