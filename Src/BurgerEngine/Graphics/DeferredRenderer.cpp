#include "BurgerEngine/Graphics/DeferredRenderer.h"

#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/SceneGraph.h"

#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Graphics/OpenGLContext.h"
#include "BurgerEngine/Graphics/FBO.h"
#include "BurgerEngine/Graphics/ShaderManager.h"
#include "BurgerEngine/Graphics/TextureManager.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/Texture2D.h"
#include "BurgerEngine/Graphics/RenderingContext.h"
#include "BurgerEngine/Graphics/ParticleRenderer.h"
#include "BurgerEngine/Graphics/Material.h"

#include "BurgerEngine/Core/Timer.h"

#include "BurgerEngine/External/Math/Vector.h"
#include "BurgerEngine/External/Math/Miniball.h"
#include "BurgerEngine/External/Math/Frustum.h"
#include "BurgerEngine/External/GLFont/glfont.h"

#include <sstream>
#include <algorithm>

#include "BurgerEngine/Graphics/StaticMesh.h"

#include "BurgerEngine/Graphics/SkyBox.h"


const int GLOW_RATIO = 4;
const float MAX_FRAME = 15.0;

const int PROFILING_LEFT_OFFSET = 300;

//TMP CASCADED SHADOWMAPS VARIABLES
vec3 pFrustumPoints[32];
float pSphere[16];

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
DeferredRenderer::DeferredRenderer()
	: m_iDebugFlag(0)
	, m_iSkipCulling(0)
	, m_iDebugBoundingBox(0)
	, m_iDebugRender(0)
	, m_iDebugSpotLightFrustum(0)
	, m_fFrameTime( 10.0 )
	, m_bShowDebugMenu( false )
	, m_fToneMappingKey( 0.5f )
	, m_fGlowMultiplier( 1.0f )
	, m_fBrightPassThreshold( 0.5f )
	, m_fBrightPassOffset( 10.0f )
	, m_fAdaptationBaseTime( 0.0f )
	, m_pDirectionalShadowLight( NULL )
	//, m_pFrustumPoints( NULL )
	, m_pFrustumBoundingSpheres( NULL )
	, bComputeFrustumBoundingSpheres( true )
	, m_pColorLUT( NULL )

{
	CreateFBO();
	
	//loading engine shaders
	LoadEngineShaders();
	
	//loading font
	GLuint iFontId;
	glGenTextures(1, &iFontId);
	m_pFont = new PixelPerfectGLFont();
	m_pFont->Create("../Data/Fonts/lucida_sans.glf", iFontId);

	GLuint iFontId2;
	glGenTextures(1, &iFontId2);
	m_pFont2 = new PixelPerfectGLFont();
	m_pFont2->Create("../Data/Fonts/lucida_sans_bold.glf", iFontId2);

	m_oTimer = new Timer();
	m_fFrameCount = MAX_FRAME;

	//m_pFrustumPoints = new vec3[DirectionalLight::iCascadeCount * 8];
	m_pFrustumBoundingSpheres = new float[DirectionalLight::iCascadeCount * 4];

	GenFullScreenQuad();

	DebugMenu& oDebugMenu = Engine::GrabInstance().GrabRenderContext().GetDebugMenu();

	oDebugMenu.AddEntry( "ToneMappingKey", m_fToneMappingKey, 0.0f, 10.0f, 0.01f );
	oDebugMenu.AddEntry( "Glow Multiplier", m_fGlowMultiplier, 0.0f, 10.0f, 0.5f );
	oDebugMenu.AddEntry( "BrightPass Threshold", m_fBrightPassThreshold, 0.0f, 50.0f, 0.5f );
	oDebugMenu.AddEntry( "BrightPass Offset", m_fBrightPassOffset, 0.0f, 50.0f, 0.5f );
	oDebugMenu.AddEntry( "Adaptation Base Time", m_fAdaptationBaseTime, 0.0f, 1.0f, 0.02f );
	oDebugMenu.AddEntry( "DebugFlag", m_iDebugFlag, 0, 100, 1 );
	oDebugMenu.AddEntry( "Skip Culling", m_iSkipCulling, 0, 1, 1 );
	oDebugMenu.AddEntry( "Show Bounding Boxes", m_iDebugBoundingBox, 0, 1, 1 );
	oDebugMenu.AddEntry( "Show Spot Lights Frustum", m_iDebugSpotLightFrustum, 0, 1, 1 );
	oDebugMenu.AddEntry( "DebugRender", m_iDebugRender, 0, 10, 1 );

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::SetPostProcessParameters( float fToneMappingKey, float fGlowMultiplier, float fBrightPassThreshold, float fBrightPassOffset, float fAdaptationBaseTime, const char* pColorLUT )
{
	m_fToneMappingKey = fToneMappingKey;
	m_fGlowMultiplier = fGlowMultiplier;
	m_fBrightPassThreshold = fBrightPassThreshold;
	m_fBrightPassOffset = fBrightPassOffset;
	m_fAdaptationBaseTime = fAdaptationBaseTime;
	m_pColorLUT = static_cast< Texture3D* >( TextureManager::GrabInstance().AddTexture( pColorLUT ) );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
DeferredRenderer::~DeferredRenderer()
{
	delete m_pGBuffer;
	m_pGBuffer = NULL;

	delete m_pLightBuffer;
	m_pLightBuffer = NULL;

	delete m_pHDRSceneBuffer;
	m_pHDRSceneBuffer = NULL;

	delete m_pLDRSceneBuffer;
	m_pLDRSceneBuffer = NULL;

	delete m_pLDRSceneBuffer2;
	m_pLDRSceneBuffer = NULL;

	delete m_pSpotShadowBlurBuffer;
	m_pSpotShadowBlurBuffer = NULL;

	delete m_pDownSampledSceneBuffer;
	m_pDownSampledSceneBuffer = NULL;

	delete m_p64x64LumBuffer;
	m_p64x64LumBuffer = NULL;
	
	delete m_p16x16LumBuffer;
	m_p16x16LumBuffer = NULL;
	
	delete m_p4x4LumBuffer;
	m_p4x4LumBuffer = NULL;
	
	delete m_p1x1LumBuffer;
	m_p1x1LumBuffer = NULL;

	delete m_pCurrentAdaptationBuffer;
	m_pCurrentAdaptationBuffer = NULL;

	delete m_pLastAdaptationBuffer;
	m_pLastAdaptationBuffer = NULL;

	delete m_pBrightPass1Buffer;
	m_pBrightPass1Buffer = NULL;

	delete m_pBrightPass2Buffer;
	m_pBrightPass2Buffer = NULL;

	delete m_pDOFBlur1Buffer;
	m_pDOFBlur1Buffer = NULL;

	delete m_pDOFBlur2Buffer;
	m_pDOFBlur2Buffer = NULL;

	delete m_pFont;
	m_pFont = NULL;

	delete m_pFont2;
	m_pFont2 = NULL;

	delete m_oTimer;
	m_oTimer = NULL;

	/*delete [] m_pFrustumPoints;
	m_pFrustumPoints = NULL;*/

	delete [] m_pFrustumBoundingSpheres;
	m_pFrustumBoundingSpheres = NULL;

	glDeleteBuffers( 1, &m_iFullScreenQuadBufferId );
	glDeleteBuffers( 1, &m_iFullScreenQuadBufferIdCW );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::CreateFBO()
{
	Engine const& rEngine = Engine::GetInstance();

	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	m_pGBuffer = new FBO( iWindowWidth, iWindowHeight, FBO::E_FBO_2D );
	m_pGBuffer->Generate( GL_RGBA16F_ARB );

	m_pLightBuffer = new FBO( iWindowWidth, iWindowHeight, FBO::E_FBO_2D );
	m_pLightBuffer->GenerateColorOnly( GL_RGBA16F_ARB );

	m_pHDRSceneBuffer = new FBO( iWindowWidth, iWindowHeight, FBO::E_FBO_2D );
	m_pHDRSceneBuffer->GenerateFinalHDRBuffer();

	m_pLDRSceneBuffer = new FBO( iWindowWidth, iWindowHeight, FBO::E_FBO_2D );
	m_pLDRSceneBuffer->GenerateColorOnly();

	m_pLDRSceneBuffer2 = new FBO( iWindowWidth, iWindowHeight, FBO::E_FBO_2D );
	m_pLDRSceneBuffer2->GenerateColorOnly();

	m_pSpotShadowBlurBuffer = new FBO( SpotShadow::iShadowMapSize, SpotShadow::iShadowMapSize, FBO::E_FBO_2D );
	m_pSpotShadowBlurBuffer->GenerateColorOnly( GL_RGBA32F_ARB );

	m_pDownSampledSceneBuffer = new FBO( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO, FBO::E_FBO_2D );
	m_pDownSampledSceneBuffer->GenerateColorOnly( GL_RGBA16F_ARB );

	m_p64x64LumBuffer = new FBO( 64, 64, FBO::E_FBO_2D );
	m_p64x64LumBuffer->GenerateColorOnly( GL_INTENSITY16F_ARB );
	
	m_p16x16LumBuffer = new FBO( 16, 16, FBO::E_FBO_2D );
	m_p16x16LumBuffer->GenerateColorOnly( GL_INTENSITY16F_ARB );
	
	m_p4x4LumBuffer = new FBO( 4, 4, FBO::E_FBO_2D );
	m_p4x4LumBuffer->GenerateColorOnly( GL_INTENSITY16F_ARB );
	
	m_p1x1LumBuffer = new FBO( 1, 1, FBO::E_FBO_2D );
	m_p1x1LumBuffer->GenerateColorOnly( GL_INTENSITY16F_ARB );

	m_pCurrentAdaptationBuffer = new FBO( 1, 1, FBO::E_FBO_2D );
	m_pCurrentAdaptationBuffer->GenerateColorOnly( GL_INTENSITY16F_ARB );
	m_pCurrentAdaptationBuffer->Activate();
	glClear( GL_COLOR_BUFFER_BIT );
	m_pCurrentAdaptationBuffer->Deactivate();

	m_pLastAdaptationBuffer = new FBO( 1, 1, FBO::E_FBO_2D );
	m_pLastAdaptationBuffer->GenerateColorOnly( GL_INTENSITY16F_ARB );
	m_pLastAdaptationBuffer->Activate();
	glClear( GL_COLOR_BUFFER_BIT );
	m_pLastAdaptationBuffer->Deactivate();

	m_pBrightPass1Buffer = new FBO( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO, FBO::E_FBO_2D );
	m_pBrightPass1Buffer->GenerateColorOnly();

	m_pBrightPass2Buffer = new FBO( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO, FBO::E_FBO_2D );
	m_pBrightPass2Buffer->GenerateColorOnly();

	m_pDOFBlur1Buffer = new FBO( iWindowWidth/2, iWindowHeight/2, FBO::E_FBO_2D );
	m_pDOFBlur1Buffer->GenerateColorOnly();

	m_pDOFBlur2Buffer = new FBO( iWindowWidth/2, iWindowHeight/2, FBO::E_FBO_2D );
	m_pDOFBlur2Buffer->GenerateColorOnly();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::LoadEngineShaders()
{
	ShaderManager& rShaderManager = ShaderManager::GrabInstance();
	
	//Directional light shader
	m_pDirectionalLightShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/DirectionalLight.bfx.xml" );
	m_pDirectionalLightShader->Activate();

	m_pDirectionalLightShader->QueryStdUniforms();
	m_iDirectionalLightShaderInvProjHandle = glGetUniformLocation( m_pDirectionalLightShader->getHandle(), "mInvProj" );

	m_iDirectionalLightShaderShadowMatrixHandle = glGetUniformLocation( m_pDirectionalLightShader->getHandle(), "mShadowMatrices" );
	m_iDirectionalLightShaderSphereHandle = glGetUniformLocation( m_pDirectionalLightShader->getHandle(), "vSphere" );

	m_iDirectionalLightShaderColor = glGetAttribLocation( m_pDirectionalLightShader->getHandle(),"vColor");
	m_iDirectionalLightShaderViewSpacePosAndMultiplierHandle = glGetAttribLocation( m_pDirectionalLightShader->getHandle(),"vViewSpacePosAndMultiplier");

	m_pDirectionalLightShader->setUniformTexture("sNormalSampler",0);
	m_pDirectionalLightShader->setUniformTexture("sDepthSampler",1);
	m_pDirectionalLightShader->setUniformTexture("sShadowMapSampler",2);

	m_pDirectionalLightShader->Deactivate();

	//Omni light shader
	m_pOmniLightShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/OmniLight.bfx.xml" );
	m_pOmniLightShader->Activate();

	m_pOmniLightShader->QueryStdUniforms();
	m_iOmniLightShaderInvProjHandle = glGetUniformLocation( m_pOmniLightShader->getHandle(), "mInvProj" );
	
	m_iOmniLightShaderColorAndInverseRadiusHandle = glGetAttribLocation( m_pOmniLightShader->getHandle(),"vColorAndInverseRadius");
	m_iOmniLightShaderViewSpacePosAndMultiplierHandle = glGetAttribLocation( m_pOmniLightShader->getHandle(),"vViewSpacePosAndMultiplier");

	m_pOmniLightShader->setUniformTexture("sNormalSampler",0);
	m_pOmniLightShader->setUniformTexture("sDepthSampler",1);

	m_pOmniLightShader->Deactivate();

	//Spot light shader
	m_pSpotLightShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/SpotLight.bfx.xml" );
	m_pSpotLightShader->Activate();

	m_pSpotLightShader->QueryStdUniforms();
	m_iSpotLightShaderInvProjHandle = glGetUniformLocation( m_pSpotLightShader->getHandle(), "mInvProj" );
	m_iSpotLightShaderColorAndInverseRadiusHandle = glGetAttribLocation( m_pSpotLightShader->getHandle(),"vColorAndInverseRadius");
	m_iSpotLightShaderViewSpacePosAndMultiplierHandle = glGetAttribLocation( m_pSpotLightShader->getHandle(),"vViewSpacePosAndMultiplier");
	m_iSpotLightShaderViewSpaceDirHandle = glGetAttribLocation( m_pSpotLightShader->getHandle(),"vViewSpaceDir");
	m_iSpotLightShaderCosInAndOutHandle = glGetAttribLocation( m_pSpotLightShader->getHandle(),"vCosInAndOut");
	
	m_pSpotLightShader->setUniformTexture("sNormalSampler",0);
	m_pSpotLightShader->setUniformTexture("sDepthSampler",1);

	m_pSpotLightShader->Deactivate();

	//Spot shadow shader
	m_pSpotShadowShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/SpotShadow.bfx.xml" );
	m_pSpotShadowShader->Activate();

	m_pSpotShadowShader->QueryStdUniforms();
	m_iSpotShadowShaderInvProjHandle = glGetUniformLocation( m_pSpotShadowShader->getHandle(), "mInvProj" );
	m_iSpotShadowShaderShadowMatrixHandle = glGetUniformLocation( m_pSpotShadowShader->getHandle(), "mShadowMatrix" );

	m_iSpotShadowShaderColorAndInverseRadiusHandle = glGetAttribLocation( m_pSpotShadowShader->getHandle(),"vColorAndInverseRadius");
	m_iSpotShadowShaderViewSpacePosAndMultiplierHandle = glGetAttribLocation( m_pSpotShadowShader->getHandle(),"vViewSpacePosAndMultiplier");
	m_iSpotShadowShaderViewSpaceDirHandle = glGetAttribLocation( m_pSpotShadowShader->getHandle(),"vViewSpaceDir");
	m_iSpotShadowShaderCosInAndOutHandle = glGetAttribLocation( m_pSpotShadowShader->getHandle(),"vCosInAndOut");
	
	m_pSpotShadowShader->setUniformTexture("sNormalSampler",0);
	m_pSpotShadowShader->setUniformTexture("sDepthSampler",1);
	m_pSpotShadowShader->setUniformTexture("sShadowMapSampler",2);
	m_pSpotShadowShader->Deactivate();

	//Exponential ShadowMap Shader
	m_pExponentialShadowMapShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/ExponentialShadowMap.bfx.xml" );
	m_pExponentialShadowMapShader->Activate();
	m_pExponentialShadowMapShader->QueryStdUniforms();
	m_pExponentialShadowMapShader->Activate();

	//Exponential ShadowMap Shader
	m_pShadowMapShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/ShadowMap.bfx.xml" );
	m_pShadowMapShader->Activate();
	m_pShadowMapShader->QueryStdUniforms();
	m_pShadowMapShader->Activate();

	//Blur 6 shader
	m_pBlur6Shader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/GaussianBlur6.bfx.xml" );
	m_pBlur6Shader->Activate();
	m_pBlur6Shader->QueryStdUniforms();
	m_iBlur6ShaderPixelSizeHandle = glGetUniformLocation( m_pBlur6Shader->getHandle(), "vPixelSize" );
	m_pBlur6Shader->setUniformTexture("sTexture",0);
	m_pBlur6Shader->Deactivate();

	m_pDownSample4x4DOF = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/DownSample4x4DOF.bfx.xml" );
	m_pDownSample4x4DOF->Activate();
	m_pDownSample4x4DOF->QueryStdUniforms();
	m_iDownSample4x4DOFPixelSizeHandle = glGetUniformLocation( m_pDownSample4x4DOF->getHandle(), "vPixelSize" );
	m_pDownSample4x4DOF->setUniformTexture("sTexture",0);
	m_pDownSample4x4DOF->setUniformTexture("sBlurData",1);
	m_pDownSample4x4DOF->Deactivate();

	//Blur 10 shader
	m_pBlur10Shader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/GaussianBlur10.bfx.xml" );
	m_pBlur10Shader->Activate();
	m_pBlur10Shader->QueryStdUniforms();
	m_iBlur10ShaderPixelSizeHandle = glGetUniformLocation( m_pBlur10Shader->getHandle(), "vPixelSize" );
	m_pBlur10Shader->setUniformTexture("sTexture",0);
	m_pBlur10Shader->Deactivate();

	//Log space Blur 10 shader
	m_pLogBlur10Shader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/LogGaussianBlur10.bfx.xml" );
	m_pLogBlur10Shader->Activate();
	m_pLogBlur10Shader->QueryStdUniforms();
	m_iLogBlur10ShaderPixelSizeHandle = glGetUniformLocation( m_pLogBlur10Shader->getHandle(), "vPixelSize" );
	m_pLogBlur10Shader->setUniformTexture("sTexture",0);
	m_pLogBlur10Shader->Deactivate();

	//Downsample shader
	m_pDownSample4x4Shader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/DownSample4x4.bfx.xml" );
	m_pDownSample4x4Shader->Activate();
	m_pDownSample4x4Shader->QueryStdUniforms();
	m_pDownSample4x4Shader->setUniformTexture("sTexture",0);
	m_iDownSampleShaderPixelSizeHandle = glGetUniformLocation( m_pDownSample4x4Shader->getHandle(), "vPixelSize" );
	m_pDownSample4x4Shader->Deactivate();

	//Average Luminance shaders
	m_pAvgLumInitShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/AvgLumInit.bfx.xml" );
	m_pAvgLumInitShader->Activate();
	m_pAvgLumInitShader->QueryStdUniforms();
	m_pAvgLumInitShader->setUniformTexture("sTexture",0);
	m_iAvgLumInitShaderPixelSizeHandle = glGetUniformLocation( m_pAvgLumInitShader->getHandle(), "vPixelSize" );
	m_pAvgLumInitShader->Deactivate();

	m_pAvgLumFinalShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/AvgLumFinal.bfx.xml" );
	m_pAvgLumFinalShader->Activate();
	m_pAvgLumFinalShader->QueryStdUniforms();
	m_pAvgLumFinalShader->setUniformTexture("sTexture",0);
	m_iAvgLumFinalShaderPixelSizeHandle = glGetUniformLocation( m_pAvgLumFinalShader->getHandle(), "vPixelSize" );
	m_pAvgLumFinalShader->Deactivate();

	//Post process shaders
	m_pToneMappingShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/ToneMapping.bfx.xml" );
	m_pToneMappingShader->Activate();
	m_pToneMappingShader->QueryStdUniforms();
	m_pToneMappingShader->setUniformTexture("sTexture",0);
	m_pToneMappingShader->setUniformTexture("sLuminance",1);
	m_iToneMappingShaderKeyHandle = glGetUniformLocation( m_pToneMappingShader->getHandle(), "fToneMappingKey" );
	m_pToneMappingShader->Deactivate();

	//Post process shaders
	m_pPostProcessShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/PostProcess.bfx.xml" );
	m_pPostProcessShader->Activate();
	m_pPostProcessShader->QueryStdUniforms();
	m_pPostProcessShader->setUniformTexture("sTexture",0);
	m_pPostProcessShader->setUniformTexture("sBloom",1);
	m_pPostProcessShader->setUniformTexture("sBlurData",2);
	m_pPostProcessShader->setUniformTexture("sLUT",3);
	m_iPostProcessShaderGlowMultiplierHandle = glGetUniformLocation( m_pPostProcessShader->getHandle(), "fGlowMultiplier" );
	m_pPostProcessShader->Deactivate();

	m_pLightAdaptationShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/LightAdaptation.bfx.xml" );
	m_pLightAdaptationShader->Activate();
	m_pLightAdaptationShader->QueryStdUniforms();
	m_pLightAdaptationShader->setUniformTexture("sAvgLuminance",0);
	m_pLightAdaptationShader->setUniformTexture("sAdaptedLuminance",1);
	m_iAdaptationShaderTimeHandle = glGetUniformLocation( m_pLightAdaptationShader->getHandle(), "fElapsedAndBaseTime" );
	m_pLightAdaptationShader->Deactivate();

	m_pBrightPassShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/BrightPass.bfx.xml" );
	m_pBrightPassShader->Activate();
	m_pBrightPassShader->QueryStdUniforms();
	m_iBrightPassShaderInvViewPortHandle = glGetUniformLocation( m_pBrightPassShader->getHandle(), "vInvViewport" );
	m_iBrightPassShaderThresholdOffsetKeyHandle = glGetUniformLocation( m_pBrightPassShader->getHandle(), "fThresholdOffSetKey" );
	m_pBrightPassShader->setUniformTexture("sTexture",0);
	m_pBrightPassShader->setUniformTexture("sLuminance",1);	
	m_pBrightPassShader->Deactivate();

	m_pDOFShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/DOF.bfx.xml" );
	m_pDOFShader->Activate();
	m_pDOFShader->QueryStdUniforms();
	m_pDOFShader->setUniformTexture("sTexture",0);
	m_pDOFShader->setUniformTexture("sDownSampledTexture",1);
	m_pDOFShader->Deactivate();

	//Debug render shaders

	m_pBasicTextureShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/BasicTexture.bfx.xml" );
	m_pBasicTextureShader->Activate();
	m_pBasicTextureShader->QueryStdUniforms();
	m_pBasicTextureShader->setUniformTexture("sTexture",0);
	m_pBasicTextureShader->Deactivate();

	m_pDebugGlossShader = rShaderManager.AddShader( "../Data/Shaders/Engine/DebugRender/xml/DebugGloss.bfx.xml" );
	m_pDebugGlossShader->Activate();
	m_pDebugGlossShader->QueryStdUniforms();
	m_pDebugGlossShader->setUniformTexture("sTexture",0);
	m_pDebugGlossShader->Deactivate();

	m_pDebugSpecularShader = rShaderManager.AddShader( "../Data/Shaders/Engine/DebugRender/xml/DebugSpecular.bfx.xml" );
	m_pDebugSpecularShader->Activate();
	m_pDebugSpecularShader->QueryStdUniforms();
	m_pDebugSpecularShader->setUniformTexture("sTexture",0);
	m_pDebugSpecularShader->Deactivate();
	
	m_pDebugDepthShader = rShaderManager.AddShader( "../Data/Shaders/Engine/DebugRender/xml/DebugDepth.bfx.xml" );
	m_pDebugDepthShader->Activate();
	m_pDebugDepthShader->QueryStdUniforms();
	m_pDebugDepthShader->setUniformTexture("sTexture",0);
	m_pDebugDepthShader->Deactivate();

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::GenFullScreenQuad()
{
	Engine const& rEngine = Engine::GetInstance();
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	glGenBuffers( 1,  &m_iFullScreenQuadBufferId );
	glGenBuffers( 1,  &m_iFullScreenQuadBufferIdCW );

	unsigned int iVertexCount = 4;

	unsigned int iSizeVertex = ( iVertexCount * sizeof( vec2 ) );
	vec2 * pVertex = new vec2[ iVertexCount ];
	
	pVertex[0] = vec2( 1.0f, 1.0f );
	pVertex[1] = vec2( 0.0f, 1.0f );
	pVertex[2] = vec2( 0.0f, 0.0f );
	pVertex[3] = vec2( 1.0f, 0.0f );

	glBindBuffer( GL_ARRAY_BUFFER, m_iFullScreenQuadBufferId );

	glBufferData( GL_ARRAY_BUFFER, 2 * iSizeVertex, 0, GL_STATIC_DRAW );
	glBufferSubData(GL_ARRAY_BUFFER, 0, iSizeVertex, pVertex );
	glBufferSubData(GL_ARRAY_BUFFER, iSizeVertex, iSizeVertex, pVertex );

	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	pVertex[0] = vec2( 1.0f, 1.0f );
	pVertex[3] = vec2( 0.0f, 1.0f );
	pVertex[2] = vec2( 0.0f, 0.0f );
	pVertex[1] = vec2( 1.0f, 0.0f );

	glBindBuffer( GL_ARRAY_BUFFER, m_iFullScreenQuadBufferIdCW );

	glBufferData( GL_ARRAY_BUFFER, 2 * iSizeVertex, 0, GL_STATIC_DRAW );
	glBufferSubData(GL_ARRAY_BUFFER, 0, iSizeVertex, pVertex );
	glBufferSubData(GL_ARRAY_BUFFER, iSizeVertex, iSizeVertex, pVertex );

	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	delete [] pVertex;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::DrawFullScreenQuad( int iWindowWidth, int iWindowHeight, bool bCCW )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	Engine& rEngine = Engine::GrabInstance();
	rEngine.GrabRenderingContext().ReshapeGlOrtho( iWindowWidth, iWindowHeight );
	RenderingContext& rRenderContext = rEngine.GrabRenderContext();

	float4x4 oOrthoMatrix = orthoMatrix(0.0, static_cast<float>(iWindowWidth), 0, static_cast<float>(iWindowHeight),-0.2f,0.2f);
	rRenderContext.PushMVP(oOrthoMatrix * scale(static_cast<float>(iWindowWidth), static_cast<float>(iWindowHeight), 0.0f));
	rRenderContext.GetCurrentShader()->CommitStdUniforms();
	
	if( bCCW )
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_iFullScreenQuadBufferId);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_iFullScreenQuadBufferIdCW);
	}

	glEnableClientState( GL_VERTEX_ARRAY );	
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer(2, GL_FLOAT, 0, 0);

	glClientActiveTexture( GL_TEXTURE0 );
	glTexCoordPointer(2, GL_FLOAT, 0, (void*)(4 * sizeof( vec2 ) ) );

	glDrawArrays(GL_QUADS, 0, 4 );

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer(GL_ARRAY_BUFFER, 0 );
	rRenderContext.PopMVP();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::RenderDirectionalLights( std::vector< SceneLight::SceneLightQuad > vDirectionalLightQuads )
{
	Engine const& rEngine = Engine::GetInstance();
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();
	
	float fHalfWidth = iWindowWidth * 0.5f; 
	float fHalfHeight = iWindowHeight * 0.5f; 

	GLuint iVertexBufferId;	
	glGenBuffers(1,  &iVertexBufferId);

	unsigned int iVertexCount = vDirectionalLightQuads.size() * 4;
	unsigned int iSizeOfDirectionalVertex = sizeof(SceneLight::DirectionalLightVertex);

	unsigned int iSizeDirectionalVertex = ( iVertexCount * iSizeOfDirectionalVertex );
	SceneLight::DirectionalLightVertex * pDirectionalVertex = new SceneLight::DirectionalLightVertex[ iVertexCount ];

	for(unsigned int i = 0; i < vDirectionalLightQuads.size(); ++i )
	{
		float fRight = (float)iWindowWidth;
		float fLeft = 0.0f;
		float fTop = (float)iWindowHeight;
		float fBottom = 0.0f;

		SceneLight::DirectionalLightVertex oTopRight, oBottomRight, oBottomLeft, oTopLeft;
		
		oTopRight.vColor = vec3( vDirectionalLightQuads[i].vColor.x, vDirectionalLightQuads[i].vColor.y, vDirectionalLightQuads[i].vColor.z );
		oTopRight.vViewSpaceLightPos = vec3( vDirectionalLightQuads[i].vViewSpaceLightPos.x, vDirectionalLightQuads[i].vViewSpaceLightPos.y, vDirectionalLightQuads[i].vViewSpaceLightPos.z );
		oTopRight.fMultiplier = vDirectionalLightQuads[i].fMultiplier;

		oBottomRight = oBottomLeft = oTopLeft = oTopRight;

		oBottomRight.vScreenSpaceVertexPos = vec2( fRight, fBottom );
		oBottomLeft.vScreenSpaceVertexPos = vec2( fLeft, fBottom );
		oTopLeft.vScreenSpaceVertexPos = vec2( fLeft, fTop);
		oTopRight.vScreenSpaceVertexPos = vec2( fRight, fTop );

		pDirectionalVertex[i*4] = oTopRight;
		pDirectionalVertex[i*4+1] = oTopLeft;
		pDirectionalVertex[i*4+2] = oBottomLeft;
		pDirectionalVertex[i*4+3] = oBottomRight;
	}

	glBindBuffer(GL_ARRAY_BUFFER, iVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, iSizeDirectionalVertex, pDirectionalVertex, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(m_iDirectionalLightShaderColor);
	glEnableVertexAttribArray(m_iDirectionalLightShaderViewSpacePosAndMultiplierHandle);

	glVertexPointer(2, GL_FLOAT, iSizeOfDirectionalVertex, 0);

	glVertexAttribPointer(m_iDirectionalLightShaderColor, 3, GL_FLOAT, GL_FALSE, iSizeOfDirectionalVertex, (void*)( 2 * sizeof(float) ) );	
	glVertexAttribPointer(m_iDirectionalLightShaderViewSpacePosAndMultiplierHandle, 4, GL_FLOAT, GL_FALSE, iSizeOfDirectionalVertex, (void*)( 5 * sizeof(float) ) );	

	glDrawArrays(GL_QUADS, 0, iVertexCount );

	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableVertexAttribArray(m_iDirectionalLightShaderColor);
	glDisableVertexAttribArray(m_iDirectionalLightShaderViewSpacePosAndMultiplierHandle);

	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	glDeleteBuffers(1, &iVertexBufferId);

	delete [] pDirectionalVertex;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::RenderOmniLights( std::vector< OmniLight::OmniLightQuad > vOmniLightQuads )
{
	Engine const& rEngine = Engine::GetInstance();
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();
	
	float fHalfWidth = iWindowWidth * 0.5f; 
	float fHalfHeight = iWindowHeight * 0.5f; 

	GLuint iVertexBufferId;	
	glGenBuffers(1,  &iVertexBufferId);

	unsigned int iVertexCount = vOmniLightQuads.size() * 4;
	unsigned int iSizeOfOmniVertex = sizeof(OmniLight::OmniLightVertex);

	unsigned int iSizeOmniVertex = ( iVertexCount * iSizeOfOmniVertex );
	OmniLight::OmniLightVertex * pOmniVertex = new OmniLight::OmniLightVertex[ iVertexCount ];

	for(unsigned int i = 0; i < vOmniLightQuads.size(); ++i )
	{
		float x = fHalfWidth * (vOmniLightQuads[i].vScreenSpaceQuadCenter.x + 1.0f);
		float y = fHalfHeight * (vOmniLightQuads[i].vScreenSpaceQuadCenter.y + 1.0f);

		float fHalfSquare = fHalfWidth * vOmniLightQuads[i].fHalfWidth;

		float fRight = clamp(x+fHalfSquare,0.0f, (float)iWindowWidth);
		float fLeft = clamp(x-fHalfSquare,0.0f, (float)iWindowWidth);
		float fTop = clamp(y+fHalfSquare,0.0f, (float)iWindowHeight);
		float fBottom = clamp(y-fHalfSquare,0.0f, (float)iWindowHeight);

		OmniLight::OmniLightVertex oTopRight, oBottomRight, oBottomLeft, oTopLeft;
		
		oTopRight.vColor = vec3( vOmniLightQuads[i].vColor.x, vOmniLightQuads[i].vColor.y, vOmniLightQuads[i].vColor.z );
		oTopRight.fInverseRadius = vOmniLightQuads[i].fInverseRadius;
		oTopRight.vViewSpaceLightPos = vec3( vOmniLightQuads[i].vViewSpaceLightPos.x, vOmniLightQuads[i].vViewSpaceLightPos.y, vOmniLightQuads[i].vViewSpaceLightPos.z );
		oTopRight.fMultiplier = vOmniLightQuads[i].fMultiplier;

		oBottomRight = oBottomLeft = oTopLeft = oTopRight;

		oBottomRight.vScreenSpaceVertexPos = vec2( fRight, fBottom );
		oBottomLeft.vScreenSpaceVertexPos = vec2( fLeft, fBottom );
		oTopLeft.vScreenSpaceVertexPos = vec2( fLeft, fTop);
		oTopRight.vScreenSpaceVertexPos = vec2( fRight, fTop );

		pOmniVertex[i*4] = oTopRight;
		pOmniVertex[i*4+1] = oTopLeft;
		pOmniVertex[i*4+2] = oBottomLeft;
		pOmniVertex[i*4+3] = oBottomRight;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, iVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, iSizeOmniVertex, pOmniVertex, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(m_iOmniLightShaderColorAndInverseRadiusHandle);
	glEnableVertexAttribArray(m_iOmniLightShaderViewSpacePosAndMultiplierHandle);

	glVertexPointer(2, GL_FLOAT, iSizeOfOmniVertex, 0);

	glVertexAttribPointer(m_iOmniLightShaderColorAndInverseRadiusHandle, 4, GL_FLOAT, GL_FALSE, iSizeOfOmniVertex, (void*)( 2 * sizeof(float) ) );	
	glVertexAttribPointer(m_iOmniLightShaderViewSpacePosAndMultiplierHandle, 4, GL_FLOAT, GL_FALSE, iSizeOfOmniVertex, (void*)( 6 * sizeof(float) ) );	

	glDrawArrays(GL_QUADS, 0, iVertexCount );

	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableVertexAttribArray(m_iOmniLightShaderColorAndInverseRadiusHandle);
	glDisableVertexAttribArray(m_iOmniLightShaderViewSpacePosAndMultiplierHandle);

	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	glDeleteBuffers(1, &iVertexBufferId);

	delete [] pOmniVertex;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------	
void DeferredRenderer::RenderSpotLights( std::vector< SpotLight::SpotLightQuad > vSpotLightQuads, unsigned int iColorAndInverseRadiusHandle, unsigned int iViewSpacePosAndMultiplierHandle, unsigned int iViewSpaceDirHandle, unsigned int iCosInAndOutHandle )
{
	Engine const& rEngine = Engine::GetInstance();
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	float fHalfWidth = iWindowWidth * 0.5f;
	float fHalfHeight = iWindowHeight * 0.5f; 

	GLuint iVertexBufferId;	
	glGenBuffers(1,  &iVertexBufferId);

	unsigned int iVertexCount = vSpotLightQuads.size() * 4;
	unsigned int iSizeOfSpotVertex = sizeof(SpotLight::SpotLightVertex);

	unsigned int iSizeSpotVertex = ( iVertexCount * iSizeOfSpotVertex );
	SpotLight::SpotLightVertex * pOmniVertex = new SpotLight::SpotLightVertex[ iVertexCount ];

	for(unsigned int i = 0; i < vSpotLightQuads.size(); ++i )
	{
		float fLeft = fHalfWidth * (vSpotLightQuads[i].vLeftRightTopBottom.x + 1.0f);
		float fRight = fHalfWidth * (vSpotLightQuads[i].vLeftRightTopBottom.y + 1.0f);
		float fTop = fHalfHeight * (vSpotLightQuads[i].vLeftRightTopBottom.z + 1.0f);
		float fBottom = fHalfHeight * (vSpotLightQuads[i].vLeftRightTopBottom.w + 1.0f);

		fRight = clamp( fRight ,0.0f, (float)iWindowWidth);
		fLeft = clamp( fLeft, 0.0f, (float)iWindowWidth);
		fTop = clamp( fTop, 0.0f, (float)iWindowHeight);
		fBottom = clamp( fBottom, 0.0f, (float)iWindowHeight);

		SpotLight::SpotLightVertex oTopRight, oBottomRight, oBottomLeft, oTopLeft;
		
		oTopRight.vColor = vec3( vSpotLightQuads[i].vColor.x, vSpotLightQuads[i].vColor.y, vSpotLightQuads[i].vColor.z );
		oTopRight.fInverseRadius = vSpotLightQuads[i].fInverseRadius;
		oTopRight.vViewSpaceLightPos = vec3( vSpotLightQuads[i].vViewSpaceLightPos.x, vSpotLightQuads[i].vViewSpaceLightPos.y, vSpotLightQuads[i].vViewSpaceLightPos.z );
		oTopRight.fMultiplier = vSpotLightQuads[i].fMultiplier;
		oTopRight.vViewSpaceLightDir = vSpotLightQuads[i].vViewSpaceLightDir;
		oTopRight.vCosInAndOut = vSpotLightQuads[i].vCosInAndOut;

		oBottomRight = oBottomLeft = oTopLeft = oTopRight;

		oBottomRight.vScreenSpaceVertexPos = vec2( fRight, fBottom );
		oBottomLeft.vScreenSpaceVertexPos = vec2( fLeft, fBottom );
		oTopLeft.vScreenSpaceVertexPos = vec2( fLeft, fTop);
		oTopRight.vScreenSpaceVertexPos = vec2( fRight, fTop );

		pOmniVertex[i*4] = oTopRight;
		pOmniVertex[i*4+1] = oTopLeft;
		pOmniVertex[i*4+2] = oBottomLeft;
		pOmniVertex[i*4+3] = oBottomRight;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, iVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, iSizeSpotVertex, pOmniVertex, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray( iColorAndInverseRadiusHandle );
	glEnableVertexAttribArray( iViewSpacePosAndMultiplierHandle );
	glEnableVertexAttribArray( iViewSpaceDirHandle );
	glEnableVertexAttribArray( iCosInAndOutHandle );

	glVertexPointer(2, GL_FLOAT, iSizeOfSpotVertex, 0);

	glVertexAttribPointer( iColorAndInverseRadiusHandle, 4, GL_FLOAT, GL_FALSE, iSizeOfSpotVertex, (void*)( 2 * sizeof(float) ) );	
	glVertexAttribPointer( iViewSpacePosAndMultiplierHandle, 4, GL_FLOAT, GL_FALSE, iSizeOfSpotVertex, (void*)( 6 * sizeof(float) ) );	
	glVertexAttribPointer( iViewSpaceDirHandle, 3, GL_FLOAT, GL_FALSE, iSizeOfSpotVertex, (void*)( 10 * sizeof(float) ) );
	glVertexAttribPointer( iCosInAndOutHandle, 2, GL_FLOAT, GL_FALSE, iSizeOfSpotVertex, (void*)( 13 * sizeof(float) ) );

	glDrawArrays(GL_QUADS, 0, iVertexCount );

	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableVertexAttribArray( iColorAndInverseRadiusHandle );
	glDisableVertexAttribArray( iViewSpacePosAndMultiplierHandle );
	glDisableVertexAttribArray( iViewSpaceDirHandle );
	glDisableVertexAttribArray( iCosInAndOutHandle );
	
	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	glDeleteBuffers(1, &iVertexBufferId);

	delete [] pOmniVertex;

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::DrawScreenSpaceQuad( int iWindowWidth, int iWindowHeight, vec3 vData )
{
	glPointSize(3);
	glDisable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,0,iWindowHeight,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float fHalfWidth = iWindowWidth * 0.5f;
	float fHalfHeight = iWindowHeight * 0.5f;

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//center
	float x = fHalfWidth * (vData.x + 1);
	float y = fHalfHeight * (vData.y + 1);

	float fHalfSquare = fHalfWidth * vData.z;

	float fRight = clamp(x+fHalfSquare,0.0f, (float)iWindowWidth);
	float fLeft = clamp(x-fHalfSquare,0.0f, (float)iWindowWidth);
	float fTop = clamp(y+fHalfSquare,0.0f, (float)iWindowHeight);
	float fBottom = clamp(y-fHalfSquare,0.0f, (float)iWindowHeight);

	glBegin(GL_POINTS);
	glVertex2f(x, y );	
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(fRight, fTop );
	glVertex2f(fRight,fBottom );
	glVertex2f(fLeft, fBottom );
	glVertex2f(fLeft, fTop );

	glEnd();
	
	glEnable(GL_CULL_FACE);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void DeferredRenderer::DrawFrustum( const vec3 * pPoints, const vec3& f3Pos )
{
	glLineWidth( 2.0f );
	glColor3f(0.0f,1.0f,0.0f);
	glDisable( GL_DEPTH_TEST );

	glBegin( GL_LINES );
	for(unsigned int i = 0; i < 4; ++i)
	{
		glVertex3f(f3Pos.x, f3Pos.y, f3Pos.z);
		glVertex3f(pPoints[i].x, pPoints[i].y, pPoints[i].z);
	}
				
	glVertex3f(pPoints[0].x, pPoints[0].y, pPoints[0].z);
	glVertex3f(pPoints[1].x, pPoints[1].y, pPoints[1].z);
			
	glVertex3f(pPoints[0].x, pPoints[0].y, pPoints[0].z);
	glVertex3f(pPoints[2].x, pPoints[2].y, pPoints[2].z);

	glVertex3f(pPoints[2].x, pPoints[2].y, pPoints[2].z);
	glVertex3f(pPoints[3].x, pPoints[3].y, pPoints[3].z);

	glVertex3f(pPoints[1].x, pPoints[1].y, pPoints[1].z);
	glVertex3f(pPoints[3].x, pPoints[3].y, pPoints[3].z);

	glEnd();

	glEnable( GL_DEPTH_TEST );
}

void DeferredRenderer::DrawCube( const vec3 * pPoints )
{
	glLineWidth( 3.0f );
	
	glBegin( GL_LINES );

	for(unsigned int i = 0; i < 4; ++i )
	{
		glVertex3f(pPoints[i].x, pPoints[i].y, pPoints[i].z);
		if( i != 3 )
		{
			glVertex3f(pPoints[i+1].x, pPoints[i+1].y, pPoints[i+1].z);
		}
		else
		{
			glVertex3f(pPoints[0].x, pPoints[0].y, pPoints[0].z);
		}		
	}
	
	for(unsigned int i = 4; i < 8; ++i )
	{
		glVertex3f(pPoints[i].x, pPoints[i].y, pPoints[i].z);
		if( i != 7 )
		{
			glVertex3f(pPoints[i+1].x, pPoints[i+1].y, pPoints[i+1].z);
		}
		else
		{
			glVertex3f(pPoints[4].x, pPoints[4].y, pPoints[4].z);
		}
	}
	
	for(unsigned int i = 0; i < 4; ++i )
	{
		glVertex3f(pPoints[i].x, pPoints[i].y, pPoints[i].z);
		glVertex3f(pPoints[i+4].x, pPoints[i+4].y, pPoints[i+4].z);
	}
	
	glEnd();
	
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::DisplayText( const std::string& sText, int iPosX, int iPosY, PixelPerfectGLFont* oFont )
{
	Engine const& rEngine = Engine::GetInstance();
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,iWindowHeight,0,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture( GL_TEXTURE0 );
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);

	oFont->Begin();
	oFont->TextOut( sText, iPosX, iPosY, 0);
	glDisable(GL_TEXTURE_2D);
	Texture2D::Deactivate();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::DisplayDebugMenu()
{
	const DebugMenu& oDebugMenu = Engine::GrabInstance().GrabRenderContext().GetDebugMenu();
	const std::pair< std::string, DebugMenuEntryBase* > oEntryPair = oDebugMenu.GetEntry();
	
	std::stringstream oStream;
	oStream << "Debug Menu ";
	DisplayText( oStream.str(), 200, 70, m_pFont2 );

	DebugMenuEntryBase* oEntry = oEntryPair.second;

	std::stringstream oStream2;
	oStream2 << oEntryPair.first << ": " << oEntry->GetValue();
	DisplayText( oStream2.str(), 200, 90, m_pFont);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::PrepareDirectionalLights( std::vector< DirectionalLight* > const& oSceneLights, AbstractCamera const& rCamera, float4x4 const& mView )
{
	m_vDirectionalLightQuads.clear();
	std::vector< DirectionalLight* >::const_iterator oLightIt = oSceneLights.begin();
	std::vector< DirectionalLight* >::const_iterator oLightItEnd = oSceneLights.end();

	while( oLightIt != oLightItEnd )
	{
		SceneLight * pLight = (*oLightIt);
		vec3 vRotation = pLight->GetRotation();
		vec4 f4Pos = rotateY( vRotation.y * DEG_TO_RAD ) * rotateX( vRotation.x * DEG_TO_RAD ) * vec4( 0.0f, 0.0f, -1.0f, 0.0f );//pLight->GetPos();

		SceneLight::SceneLightQuad oQuad;

		//computing view space position
		vec4 vViewSpacePos = mView * f4Pos;
		oQuad.vViewSpaceLightPos = vec3( vViewSpacePos.x, vViewSpacePos.y, vViewSpacePos.z );
		oQuad.vColor = pLight->GetColor();
		oQuad.fMultiplier = pLight->GetMultiplier();

		m_vDirectionalLightQuads.push_back( oQuad );
		++oLightIt;
	}
	m_iDirectionalCount = oSceneLights.size();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::PrepareOmniLights( const std::vector< OmniLight* >& oOmniLights, const AbstractCamera & rCamera, const float4x4& mView, const float4x4& mViewProjection )
{
	m_vOmniLightQuads.clear();
	std::vector< OmniLight* >::const_iterator oLightIt = oOmniLights.begin();
	std::vector< OmniLight* >::const_iterator oLightItEnd = oOmniLights.end();

	while( oLightIt != oLightItEnd )
	{
		OmniLight * pLight = (*oLightIt);
		
		vec3 f3Pos = pLight->GetPos();
		float fRadius = pLight->GetRadius(); 
		
		{
			vec3 vLightToView = rCamera.GetPos() - f3Pos;
			float fLength = length(vLightToView);

			OmniLight::OmniLightQuad oQuad;

			//computing view space position
			vec4 vViewSpacePos = mView * vec4( f3Pos.x, f3Pos.y, f3Pos.z, 1.0 );
			oQuad.vViewSpaceLightPos = vec3( vViewSpacePos.x, vViewSpacePos.y, vViewSpacePos.z );
			oQuad.vColor = pLight->GetColor();
			oQuad.fInverseRadius = 1.0f / fRadius;
			oQuad.fMultiplier = pLight->GetMultiplier();

			if( fLength <= fRadius )
			{
				oQuad.vScreenSpaceQuadCenter = vec2( 0.0f, 0.0f );
				oQuad.fHalfWidth = 1.0f;
			}
			else
			{
				vLightToView = normalize( vLightToView );

				vec3 oShiftedPos = f3Pos + min( fLength, fRadius ) * vLightToView;

				vec4 oScreenPos = mViewProjection * vec4(oShiftedPos.x, oShiftedPos.y, oShiftedPos.z, 1.0 );
				
				oScreenPos = oScreenPos / oScreenPos.w;

				vec3 vLightRight = oShiftedPos + fRadius * rCamera.GetRight();
				vec4 oScreenRightPos = mViewProjection * vec4(vLightRight.x, vLightRight.y, vLightRight.z, 1.0 );
				oScreenRightPos = oScreenRightPos / oScreenRightPos.w;

				oQuad.vScreenSpaceQuadCenter = vec2( oScreenPos.x, oScreenPos.y );
				oQuad.fHalfWidth = oScreenPos.x - oScreenRightPos.x;
			}
			m_vOmniLightQuads.push_back( oQuad );
		}
		++oLightIt;
	}
}
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::PrepareAndRenderSpotShadows( const std::vector< SpotShadow* >& oSpotShadows, const AbstractCamera & rCamera, const float4x4& mView, const float4x4& mViewProjection )
{
	std::vector< SpotShadow* >::const_iterator oLightIt = oSpotShadows.begin();
	std::vector< SpotShadow* >::const_iterator oLightItEnd = oSpotShadows.end();

	while( oLightIt != oLightItEnd )
	{
		SpotLight * pLight = (*oLightIt);
		SpotLight::SpotLightQuad oQuad;
		
		ComputeOneSpotBoundingQuad( pLight, rCamera, mView, mViewProjection, oQuad );
		
		std::vector< SpotLight::SpotLightQuad > oQuadVector;
		oQuadVector.push_back( oQuad );

		(*oLightIt)->ActivateDepthTexture();
		mat4 mShadowMatrix = (*oLightIt)->GetMatrix() * !mViewProjection;
		m_pSpotShadowShader->setUniformMatrix4fv( m_iSpotShadowShaderShadowMatrixHandle, transpose(mShadowMatrix) );

		RenderSpotLights( oQuadVector, m_iSpotShadowShaderColorAndInverseRadiusHandle, m_iSpotShadowShaderViewSpacePosAndMultiplierHandle, m_iSpotShadowShaderViewSpaceDirHandle, m_iSpotShadowShaderCosInAndOutHandle );
		
		++oLightIt;
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::PrepareSpotLights( const std::vector< SpotLight* >& oSpotLights, const AbstractCamera & rCamera, const float4x4& mView, const float4x4& mViewProjection )
{
	m_vSpotLightQuads.clear();
	std::vector< SpotLight* >::const_iterator oLightIt = oSpotLights.begin();
	std::vector< SpotLight* >::const_iterator oLightItEnd = oSpotLights.end();

	while( oLightIt != oLightItEnd )
	{
		SpotLight * pLight = (*oLightIt);
		SpotLight::SpotLightQuad oQuad;
		
		ComputeOneSpotBoundingQuad( pLight, rCamera, mView, mViewProjection, oQuad );
		
		m_vSpotLightQuads.push_back( oQuad );
		
		++oLightIt;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::ComputeOneSpotBoundingQuad( SpotLight* pLight, AbstractCamera const& rCamera, float4x4 const& mView, float4x4 const& mViewProjection, SpotLight::SpotLightQuad& oQuad )
{
	vec3 f3Pos = pLight->GetPos();
	float fRadius = pLight->GetRadius(); 

	vec3 vCameraPos = rCamera.GetPos();
	vec3 vLightToView = vCameraPos - f3Pos;

	float fLength = length(vLightToView);

	//computing view space position
	vec4 vViewSpacePos = mView * vec4( f3Pos.x, f3Pos.y, f3Pos.z, 1.0f );
	vec3 vRotation = pLight->GetRotation();

	vec4 vWorldSpaceDir = rotateY( vRotation.y * DEG_TO_RAD ) * rotateX( vRotation.x * DEG_TO_RAD ) * vec4( 0.0f, 0.0f, -1.0f, 0.0f );

	vec4 vViewSpaceDir = mView * vWorldSpaceDir;

	oQuad.vViewSpaceLightPos = vec3( vViewSpacePos.x, vViewSpacePos.y, vViewSpacePos.z );
	oQuad.vViewSpaceLightDir = vec3( vViewSpaceDir.x, vViewSpaceDir.y, vViewSpaceDir.z );
	oQuad.vColor = pLight->GetColor();
	oQuad.fInverseRadius = 1.0f / fRadius;
	oQuad.fMultiplier = pLight->GetMultiplier();
	oQuad.vCosInAndOut = vec2( pLight->GetCosInnerAngle(), pLight->GetCosOuterAngle() );

	const float * pBoundingBox = pLight->GetBoundingBox();
	if( vCameraPos.x > pBoundingBox[0] && vCameraPos.x < pBoundingBox[1] && vCameraPos.y > pBoundingBox[2] && vCameraPos.y < pBoundingBox[3] && vCameraPos.z > pBoundingBox[4] && vCameraPos.z < pBoundingBox[5] )
	{
		oQuad.vLeftRightTopBottom = vec4( -1.0f, 1.0f, 1.0f, -1.0f );
	}
	else
	{
		const vec3 * pPoints = pLight->GetFarPlanePoints();

		float fLeft, fRight, fBottom, fTop;
		vec4 oScreenPos = mViewProjection * vec4(f3Pos.x, f3Pos.y, f3Pos.z, 1.0f );
		oScreenPos = oScreenPos / abs(oScreenPos.w);
		
		fLeft = fRight = oScreenPos.x;
		fBottom = fTop = oScreenPos.y;

		for( unsigned int i = 0; i < 4; ++i )
		{
			oScreenPos = mViewProjection * vec4(pPoints[i].x, pPoints[i].y, pPoints[i].z, 1.0f );
			oScreenPos = oScreenPos / abs(oScreenPos.w);
					
			fLeft = min(fLeft, oScreenPos.x);
			fRight = max(fRight, oScreenPos.x);

			fBottom = min(fBottom, oScreenPos.y);
			fTop = max(fTop, oScreenPos.y);

			vec3 fOppositePoint = pPoints[i] - vec3(vWorldSpaceDir.x,vWorldSpaceDir.y,vWorldSpaceDir.z) * fRadius;

			oScreenPos = mViewProjection * vec4(fOppositePoint.x, fOppositePoint.y, fOppositePoint.z, 1.0f );
			oScreenPos = oScreenPos / abs(oScreenPos.w);
					
			fLeft = min(fLeft, oScreenPos.x);
			fRight = max(fRight, oScreenPos.x);

			fBottom = min(fBottom, oScreenPos.y);
			fTop = max(fTop, oScreenPos.y);

		}
		oQuad.vLeftRightTopBottom = vec4( fLeft, fRight, fTop, fBottom );
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::ComputeFrustumBoundingSpheres( const float4x4& mInvProjection, float fNear, float fFar )
{
	//computing frustum slices in view space
	//this should change only when the camera parameters or the projection parameters changes
	vec3 pFrustumPoints[DirectionalLight::iCascadeCount * 8];

	//unprojecting frustum points
	//near
	//bottom left
	vec4 f3Point = mInvProjection * vec4(-1.0f,-1.0f,fNear,1.0f);
	f3Point /= f3Point.w;
	pFrustumPoints[0] = f3Point.xyz();
	
	//bottom right
	f3Point = mInvProjection * vec4(1.0f,-1.0f,fNear,1.0f);
	f3Point /= f3Point.w;
	pFrustumPoints[1] = f3Point.xyz();

	//top right
	f3Point = mInvProjection * vec4(1.0f,1.0f,fNear,1.0f);
	f3Point /= f3Point.w;
	pFrustumPoints[2] = f3Point.xyz();

	//top left
	f3Point = mInvProjection * vec4(-1.0f,1.0f,fNear,1.0f);
	f3Point /= f3Point.w;
	pFrustumPoints[3] = f3Point.xyz();

	//far
	//bottom left
	f3Point = mInvProjection * vec4(-1.0f,-1.0f,1.0f,1.0f);
	f3Point /= f3Point.w;
	pFrustumPoints[4] = f3Point.xyz();
	
	//bottom right
	f3Point = mInvProjection * vec4(1.0f,-1.0f,1.0f,1.0f);
	f3Point /= f3Point.w;
	pFrustumPoints[5] = f3Point.xyz();

	//top right
	f3Point = mInvProjection * vec4(1.0f,1.0f,1.0f,1.0f);
	f3Point /= f3Point.w;
	pFrustumPoints[6] = f3Point.xyz();
	
	//top left
	f3Point = mInvProjection * vec4(-1.0f,1.0f,1.0f,1.0f);
	f3Point /= f3Point.w;
	pFrustumPoints[7] = f3Point.xyz();

	//computing frustum vectors
	vec3 vBottomLeft = pFrustumPoints[4] - pFrustumPoints[0];
	vBottomLeft = normalize( vBottomLeft );

	vec3 vBottomRight = pFrustumPoints[5] - pFrustumPoints[1];
	vBottomRight = normalize( vBottomRight );

	vec3 vTopRight = pFrustumPoints[6] - pFrustumPoints[2];
	vTopRight = normalize( vTopRight );

	vec3 vTopLeft = pFrustumPoints[7] - pFrustumPoints[3];
	vTopLeft = normalize( vTopLeft );

	//computing frustum slices
	float fLength = fFar * DirectionalLight::pCascadeSizes[0];
	pFrustumPoints[4] = pFrustumPoints[0] + vBottomLeft * fLength;
	pFrustumPoints[5] = pFrustumPoints[1] + vBottomRight * fLength;
	pFrustumPoints[6] = pFrustumPoints[2] + vTopRight * fLength;
	pFrustumPoints[7] = pFrustumPoints[3] + vTopLeft * fLength;

	for( unsigned int i = 1; i < DirectionalLight::iCascadeCount; ++i )
	{
		fLength = fFar * DirectionalLight::pCascadeSizes[i];
		pFrustumPoints[i*8]     = pFrustumPoints[i*8 - 4];
		pFrustumPoints[i*8 + 1] = pFrustumPoints[i*8 - 3];
		pFrustumPoints[i*8 + 2] = pFrustumPoints[i*8 - 2];
		pFrustumPoints[i*8 + 3] = pFrustumPoints[i*8 - 1];

		pFrustumPoints[i*8 + 4] = pFrustumPoints[i*8] + vBottomLeft * fLength;
		pFrustumPoints[i*8 + 5] = pFrustumPoints[i*8 + 1] + vBottomRight * fLength;
		pFrustumPoints[i*8 + 6] = pFrustumPoints[i*8 + 2] + vTopRight * fLength;
		pFrustumPoints[i*8 + 7] = pFrustumPoints[i*8 + 3] + vTopLeft * fLength;
	}
	
	//Computing frustum bounding sphere in view space
	Miniball<3> oBall;
	for( unsigned int i = 0; i < DirectionalLight::iCascadeCount; ++i )
	{
		oBall.clear();
		oBall.check_in( &pFrustumPoints[i*8],8 );
		oBall.build();

		Point<3> oCenter = oBall.center();
		vec3 vCenter = vec3(static_cast<float>(oCenter[0]),static_cast<float>(oCenter[1]),static_cast<float>(oCenter[2]));
		float fRadius = static_cast<float>(sqrt( oBall.squared_radius() ));

		//vec4 vViewSpaceCenter =  mView * vec4(vCenter,1.0f);
		m_pFrustumBoundingSpheres[i*DirectionalLight::iCascadeCount] = vCenter.x;
		m_pFrustumBoundingSpheres[i*DirectionalLight::iCascadeCount+1] = vCenter.y;
		m_pFrustumBoundingSpheres[i*DirectionalLight::iCascadeCount+2] = vCenter.z;
		m_pFrustumBoundingSpheres[i*DirectionalLight::iCascadeCount+3] = fRadius;
	}
	bComputeFrustumBoundingSpheres = false;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::RenderCascadedShadowMap( const std::vector< SceneMesh* >& oSceneMeshes, const float4x4& mInvView )
{
	Engine& rEngine = Engine::GrabInstance();
	RenderingContext& rRenderContext = rEngine.GrabRenderContext();
	
	vec3 vLightRotation = m_pDirectionalShadowLight->GetRotation();
	vec3 f3LightPosition = ( rotateY( vLightRotation.y * DEG_TO_RAD ) * rotateX( vLightRotation.x * DEG_TO_RAD ) * vec4( 0.0f, 0.0f, 2000.0f, 0.0f ) ).xyz();
	float4x4 mLightRotation = rotateXY( vLightRotation.x*DEG_TO_RAD, -(vLightRotation.y+180.0f) * DEG_TO_RAD );
	unsigned int x = 0;
	unsigned int y = 0;
	m_pDirectionalShadowLight->ActivateBuffer();
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glCullFace( GL_FRONT );
	m_pShadowMapShader->Activate();
	rRenderContext.SetCurrentShader(m_pShadowMapShader);
	for( unsigned int i = 0; i < DirectionalLight::iCascadeCount; ++i )
	{
		//world space center
		vec3 vViewCenter = vec3(m_pFrustumBoundingSpheres[i*DirectionalLight::iCascadeCount],m_pFrustumBoundingSpheres[i*DirectionalLight::iCascadeCount+1],m_pFrustumBoundingSpheres[i*DirectionalLight::iCascadeCount+2]);
		vec3 vCenter = (mInvView * vec4(vViewCenter,1.0)).xyz();

		float fRadius = m_pFrustumBoundingSpheres[i*DirectionalLight::iCascadeCount+3];
		vec3 f3Pos = f3LightPosition - vCenter;
		
		float4x4 mLightProjection = orthoMatrix(-fRadius, fRadius, -fRadius, fRadius,0.01f,10000.0f);
		float4x4 mLightView = mLightRotation * translate( f3Pos.x, f3Pos.y, f3Pos.z );

		float4x4 mLightViewProjection = mLightProjection * mLightView;

		//making sure the light move by an integral number of pixel
		vec4 vWorldOrigin = mLightViewProjection * vec4(0.0f,0.0f,0.0f,1.0f);
		vWorldOrigin *= DirectionalLight::iShadowMapSize * 0.5f;

		float fRoundedX = roundf( vWorldOrigin.x );
		float fRoundedY  = roundf( vWorldOrigin.y );

		float fDx = ( fRoundedX - vWorldOrigin.x ) / ( DirectionalLight::iShadowMapSize * 0.5f );
		float fDy = ( fRoundedY - vWorldOrigin.y ) / ( DirectionalLight::iShadowMapSize * 0.5f );

		mLightViewProjection = translate( fDx, fDy, 0.0f ) * mLightViewProjection;
		vec4 vWorldOrigin2 = mLightViewProjection * vec4(0.0f,0.0f,0.0f,1.0f);
		vWorldOrigin2 *= DirectionalLight::iShadowMapSize * 0.5f;
		m_pDirectionalShadowLight->SetMatrix( mLightViewProjection, i );
		
		glViewport(x,y,DirectionalLight::iShadowMapSize,DirectionalLight::iShadowMapSize);

		rRenderContext.PushMVP(mLightViewProjection);
		rRenderContext.PushModelView(mLightView);
		Frustum oViewFrustum;
		oViewFrustum.loadFrustum( transpose(mLightViewProjection) );

		std::vector< SceneMesh* >::const_iterator oShadowMeshIt = oSceneMeshes.begin();
		while( oShadowMeshIt != oSceneMeshes.end() )
		{
			if( (*oShadowMeshIt)->GetCastShadow() )
			{
				const float* pBoundingBox = (*oShadowMeshIt)->GetBoundingBox();

				if( m_iSkipCulling || oViewFrustum.cubeInFrustum( pBoundingBox[0], pBoundingBox[1],pBoundingBox[2],pBoundingBox[3],pBoundingBox[4],pBoundingBox[5] ) )
				{
					(*oShadowMeshIt)->Draw( EffectTechnique::E_RENDER_SHADOW_MAP );
				}
			}
			++oShadowMeshIt;
		}
		x += DirectionalLight::iShadowMapSize;
		
		rRenderContext.PopMVP();
	}
	m_pShadowMapShader->Deactivate();
	m_pDirectionalShadowLight->DeactivateBuffer();
	glCullFace( GL_BACK );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::RenderShadowMaps( const std::vector< SceneMesh* >& oSceneMeshes, const std::vector< SpotShadow* >& oSpotShadows, OpenGLContext& a_rDriverRenderingContext )
{
	Engine& rEngine = Engine::GrabInstance();
	RenderingContext& rRenderContext = rEngine.GrabRenderContext();

	std::vector< SpotShadow* >::const_iterator oSpotIt = oSpotShadows.begin();
	std::vector< SpotShadow* >::const_iterator oEnd = oSpotShadows.end();

	while( oSpotIt != oEnd )
	{
		SpotShadow * pSpot = *oSpotIt;
		
		vec3 vLightPos = pSpot->GetPos();

		vec3 vLightRotation = pSpot->GetRotation();

		float fRadius = pSpot->GetRadius();

		a_rDriverRenderingContext.Reshape( SpotShadow::iShadowMapSize, SpotShadow::iShadowMapSize, 2.0f * acosf( pSpot->GetCosOuterAngle()  ) * RAD_TO_DEG, 0.01f, fRadius );

		float4x4 mLightProjection = transpose(GlperspectiveMatrix( 2.0f * acosf( pSpot->GetCosOuterAngle()  ) * RAD_TO_DEG, 1.0f,0.5, fRadius ));
		float4x4 mLightView = rotateXY( -vLightRotation.x*DEG_TO_RAD, -vLightRotation.y*DEG_TO_RAD ) * translate( -vLightPos.x, -vLightPos.y, -vLightPos.z );
		
		float4x4 mLightViewProjection = mLightProjection * mLightView;

		rRenderContext.PushModelView(mLightView);
		rRenderContext.PushMVP(mLightViewProjection);

		pSpot->SetMatrix( mLightViewProjection );

		Frustum oViewFrustum;
		oViewFrustum.loadFrustum( transpose(mLightViewProjection) );

		m_pExponentialShadowMapShader->Activate();
		rRenderContext.SetCurrentShader(m_pExponentialShadowMapShader);
		pSpot->ActivateBuffer();

		glClearColor( fRadius * 0.001f,0.0f,0.0f,0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glClearColor( 0.0f,0.0f,0.0f,0.0f );

		std::vector< SceneMesh* >::const_iterator oShadowMeshIt = oSceneMeshes.begin();
		while( oShadowMeshIt != oSceneMeshes.end() )
		{			
			if( (*oShadowMeshIt)->GetCastShadow() )
			{
				const float* pBoundingBox = (*oShadowMeshIt)->GetBoundingBox();

				if( m_iSkipCulling || oViewFrustum.cubeInFrustum( pBoundingBox[0], pBoundingBox[1],pBoundingBox[2],pBoundingBox[3],pBoundingBox[4],pBoundingBox[5] ) )
				{
					(*oShadowMeshIt)->Draw( EffectTechnique::E_RENDER_SHADOW_MAP );
				}
			}
			++oShadowMeshIt;
		}

		rRenderContext.PopModelView();
		rRenderContext.PopMVP();

		m_pExponentialShadowMapShader->Deactivate();

		pSpot->DeactivateBuffer();
		
		pSpot->ActivateDepthTexture();
			
		m_pLogBlur10Shader->Activate();
		rRenderContext.SetCurrentShader(m_pLogBlur10Shader);
		float pPixelSize[2] = { 1.0f/ SpotShadow::iShadowMapSize, 0.0f };
		m_pLogBlur10Shader->setUniform2fv( m_iLogBlur10ShaderPixelSizeHandle, 1, pPixelSize );

		m_pSpotShadowBlurBuffer->Activate();
		DrawFullScreenQuad( SpotShadow::iShadowMapSize, SpotShadow::iShadowMapSize );

		m_pSpotShadowBlurBuffer->Deactivate();
		
		m_pSpotShadowBlurBuffer->ActivateTexture();

		pPixelSize[0] = 0.0f;
		pPixelSize[1] = 1.0f/ SpotShadow::iShadowMapSize;
		m_pLogBlur10Shader->setUniform2fv( m_iLogBlur10ShaderPixelSizeHandle, 1, pPixelSize);
			
		pSpot->ActivateBuffer();
		DrawFullScreenQuad( SpotShadow::iShadowMapSize, SpotShadow::iShadowMapSize );

		pSpot->DeactivateBuffer();

		m_pLogBlur10Shader->Deactivate();
		
		++oSpotIt;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::ComputeAvgLum()
{
	Engine & rEngine = Engine::GrabInstance();
	RenderingContext& rRenderContext = rEngine.GrabRenderContext();

	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();
	
	//Downsampling HDR Scene
	m_pDownSampledSceneBuffer->Activate();

	glActiveTexture( GL_TEXTURE0 );
	m_pHDRSceneBuffer->ActivateTexture();
	
	m_pDownSample4x4Shader->Activate();
	rRenderContext.SetCurrentShader(m_pDownSample4x4Shader);
	
	float pPixelSize[2] = { 1.0f/ ( iWindowWidth/GLOW_RATIO ), 1.0f / ( iWindowHeight/GLOW_RATIO ) };
	m_pDownSample4x4Shader->setUniform2fv( m_iDownSampleShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO );	
	
	m_pDownSample4x4Shader->Deactivate();

	m_pDownSampledSceneBuffer->Deactivate();

	//Computing average luminance
	m_p64x64LumBuffer->Activate();
	m_pAvgLumInitShader->Activate();
	rRenderContext.SetCurrentShader(m_pAvgLumInitShader);

	m_pDownSampledSceneBuffer->ActivateTexture();
	
	pPixelSize[0] = 1.0f/ 64.0f;
	pPixelSize[1] = 1.0f / 64.0f;
	m_pAvgLumInitShader->setUniform2fv( m_iAvgLumInitShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( 64, 64 );
	m_p64x64LumBuffer->Deactivate();
	
	//Downsampling to 16x16
	m_p16x16LumBuffer->Activate();
	m_pDownSample4x4Shader->Activate();
	rRenderContext.SetCurrentShader(m_pDownSample4x4Shader);

	m_p64x64LumBuffer->ActivateTexture();
	
	pPixelSize[0] = 1.0f/ 16.0f;
	pPixelSize[1] = 1.0f / 16.0f;
	m_pDownSample4x4Shader->setUniform2fv( m_iDownSampleShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( 16, 16 );
	
	//Downsampling to 4x4
	m_p4x4LumBuffer->Activate();

	m_p16x16LumBuffer->ActivateTexture();
	
	pPixelSize[0] = 1.0f/ 4.0f;
	pPixelSize[1] = 1.0f / 4.0f;
	m_pDownSample4x4Shader->setUniform2fv( m_iDownSampleShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( 4, 4 );

	//Downsampling to 1x1
	m_p1x1LumBuffer->Activate();
	m_pAvgLumFinalShader->Activate();
	rRenderContext.SetCurrentShader(m_pAvgLumFinalShader);

	m_p4x4LumBuffer->ActivateTexture();
	
	pPixelSize[0] = 1.0f;
	pPixelSize[1] = 1.0f;
	m_pAvgLumFinalShader->setUniform2fv( m_iAvgLumFinalShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( 1, 1 );
	m_p1x1LumBuffer->Deactivate();
	
	//Light Adaptation
	m_pCurrentAdaptationBuffer->Activate();
	m_pLightAdaptationShader->Activate();
	rRenderContext.SetCurrentShader(m_pLightAdaptationShader);

	float fTimes[2] = { m_fFrameTime * 0.0001f, m_fAdaptationBaseTime };
	m_pLightAdaptationShader->setUniform2fv( m_iAdaptationShaderTimeHandle, 1, fTimes );

	m_p1x1LumBuffer->ActivateTexture();
	glActiveTexture( GL_TEXTURE1 );
	m_pLastAdaptationBuffer->ActivateTexture();
	glActiveTexture( GL_TEXTURE0 );

	DrawFullScreenQuad( 1, 1 );
	m_pCurrentAdaptationBuffer->Deactivate();

	FBO * pTmpBuffer = m_pLastAdaptationBuffer;
	m_pLastAdaptationBuffer = m_pCurrentAdaptationBuffer;
	m_pCurrentAdaptationBuffer = pTmpBuffer;
	
	//Bright Pass
	m_pBrightPass1Buffer->Activate();

	m_pBrightPassShader->Activate();
	rRenderContext.SetCurrentShader(m_pBrightPassShader);
	pPixelSize[0] = 1.0f / ( iWindowWidth / GLOW_RATIO );
	pPixelSize[1] = 1.0f /( iWindowHeight / GLOW_RATIO );
	m_pBrightPassShader->setUniform2fv( m_iBrightPassShaderInvViewPortHandle, 1, pPixelSize);

	float pBrightPassUniforms[3] = { m_fBrightPassThreshold, m_fBrightPassOffset, m_fToneMappingKey };
	m_pBrightPassShader->setUniform3fv( m_iBrightPassShaderThresholdOffsetKeyHandle, 1, pBrightPassUniforms);

	m_pHDRSceneBuffer->ActivateTexture();
	glActiveTexture( GL_TEXTURE1 );
	m_pLastAdaptationBuffer->ActivateTexture();
	
	DrawFullScreenQuad( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO );
	m_pBrightPassShader->Deactivate();

	//Blur
	m_pBrightPass2Buffer->Activate();

	glActiveTexture( GL_TEXTURE0 );
	m_pBrightPass1Buffer->ActivateTexture();

	m_pBlur10Shader->Activate();
	rRenderContext.SetCurrentShader(m_pBlur10Shader);

	pPixelSize[0] = 1.0f / ( iWindowWidth / GLOW_RATIO );
	pPixelSize[1] = 0.0f;
	m_pBlur10Shader->setUniform2fv( m_iBlur10ShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO );
	
	m_pBrightPass1Buffer->Activate();
	m_pBrightPass2Buffer->ActivateTexture();

	pPixelSize[0] = 0.0f;
	pPixelSize[1] = 1.0f /( iWindowHeight / GLOW_RATIO );
	m_pBlur10Shader->setUniform2fv( m_iBlur10ShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO );

	m_pBrightPass1Buffer->Deactivate();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::GetVisibleObjects( RenderingContext& rRenderContext, const Frustum& oViewFrustum, const float4x4& mView, std::vector< SceneMesh* >& oVisibleSceneMeshes, std::vector< SceneMesh* >& oVisibleTransparentSceneMeshes, std::vector< OmniLight* >& oVisibleOmniLights, std::vector< SpotLight* >& oVisibleSpotLights, std::vector< SpotShadow* >& oVisibleSpotShadows )
{
	const std::vector< SceneMesh* >& oSceneMeshes = rRenderContext.GetSceneMeshes();
	
	std::vector< SceneMesh*>::const_iterator oIt = oSceneMeshes.begin();
	std::vector< SceneMesh*>::const_iterator oEnd = oSceneMeshes.end();
	while( oIt != oEnd )
	{
		const float * pBoundingBox = (*oIt)->GetBoundingBox();

		if(	m_iSkipCulling || oViewFrustum.cubeInFrustum( pBoundingBox[0], pBoundingBox[1],pBoundingBox[2],pBoundingBox[3],pBoundingBox[4],pBoundingBox[5] ) )
		{
			vec3 f3Pos = (*oIt)->GetPos();
			float fViewZ = ( mView * vec4( f3Pos.x, f3Pos.y, f3Pos.z, 1.0 ) ).z;
			(*oIt)->SetViewZ( fViewZ );
			oVisibleSceneMeshes.push_back( (*oIt) );
		}
		oIt++;
	}
	FrontToBackComp oFrontToBackComp;
	std::sort( oVisibleSceneMeshes.begin(), oVisibleSceneMeshes.end(), oFrontToBackComp ); 

	const std::vector< SceneMesh* >& oTransparentSceneMeshes = rRenderContext.GetTransparentSceneMeshes();
	oIt = oTransparentSceneMeshes.begin();
	oEnd = oTransparentSceneMeshes.end();
	while( oIt != oEnd )
	{
		const float * pBoundingBox = (*oIt)->GetBoundingBox();
		if(	m_iSkipCulling || oViewFrustum.cubeInFrustum( pBoundingBox[0], pBoundingBox[1],pBoundingBox[2],pBoundingBox[3],pBoundingBox[4],pBoundingBox[5] ) )
		{
			vec3 f3Pos = (*oIt)->GetPos();
			float fViewZ = ( mView * vec4( f3Pos.x, f3Pos.y, f3Pos.z, 1.0 ) ).z;
			(*oIt)->SetViewZ( fViewZ );
			oVisibleTransparentSceneMeshes.push_back( (*oIt) );
		}
		oIt++;
	}
	BackToFrontComp oBackToFrontComp;
	std::sort( oVisibleTransparentSceneMeshes.begin(), oVisibleTransparentSceneMeshes.end(), oBackToFrontComp );

	const std::vector< OmniLight* >& oOmniLights = 	rRenderContext.GetOmniLights();
	std::vector< OmniLight*>::const_iterator oOmniIt = oOmniLights.begin();
	std::vector< OmniLight*>::const_iterator oOmniEnd = oOmniLights.end();
	while( oOmniIt != oOmniEnd )
	{
		vec3 f3Pos = (*oOmniIt)->GetPos();
		float fRadius = (*oOmniIt)->GetRadius();		
		if(	m_iSkipCulling || oViewFrustum.sphereInFrustum( vec3(f3Pos.x, f3Pos.y, f3Pos.z), fRadius ) )
		{
			oVisibleOmniLights.push_back( (*oOmniIt) );
		}
		oOmniIt++;
	}

	const std::vector< SpotLight* >& oSpotLights = 	rRenderContext.GetSpotLights();
	std::vector< SpotLight*>::const_iterator oSpotIt = oSpotLights.begin();
	std::vector< SpotLight*>::const_iterator oSpotEnd = oSpotLights.end();
	while( oSpotIt != oSpotEnd )
	{
		const float * pBoundingBox = (*oSpotIt)->GetBoundingBox();
		if(	m_iSkipCulling || oViewFrustum.cubeInFrustum( pBoundingBox[0], pBoundingBox[1],pBoundingBox[2],pBoundingBox[3],pBoundingBox[4],pBoundingBox[5] ) )
		{
			oVisibleSpotLights.push_back( (*oSpotIt) );
		}
		oSpotIt++;
	}

	const std::vector< SpotShadow* >& oSpotShadows = rRenderContext.GetSpotShadows();
	std::vector< SpotShadow*>::const_iterator oShadowIt = oSpotShadows.begin();
	std::vector< SpotShadow*>::const_iterator oShadowEnd = oSpotShadows.end();
	while( oShadowIt != oShadowEnd )
	{
		const float * pBoundingBox = (*oShadowIt)->GetBoundingBox();
		if(	m_iSkipCulling || oViewFrustum.cubeInFrustum( pBoundingBox[0], pBoundingBox[1],pBoundingBox[2],pBoundingBox[3],pBoundingBox[4],pBoundingBox[5] ) )
		{
			oVisibleSpotShadows.push_back( (*oShadowIt) );
		}
		oShadowIt++;
	}

	m_iObjectCount = oSceneMeshes.size() + oTransparentSceneMeshes.size();
	m_iOmniCount = oOmniLights.size();
	m_iSpotCount = oSpotLights.size();
	m_iSpotShadowCount = oSpotShadows.size();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	
	Engine& rEngine = Engine::GrabInstance();
	RenderingContext& rRenderContext = rEngine.GrabRenderContext();
	
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	AbstractCamera & rCamera = rEngine.GetCurrentCamera();

	OpenGLContext& rHardwareRenderContext = rEngine.GrabRenderingContext(); 

	// Retrieving scene matrices
	// Todo can be used by uniform now, right?
	float4x4 mProjection = GlperspectiveMatrix( rCamera.GetFOV(), (float)iWindowWidth/(float)iWindowHeight,rCamera.GetNear(), rCamera.GetFar() );
	float4x4 mInvProjection = !mProjection;

	float4x4 mView =  rCamera.GetViewMatrix();

	float4x4 mViewProjection = transpose(mProjection) * mView;
	float4x4 mInvViewProjection = !mViewProjection;

	Frustum oViewFrustum;
	oViewFrustum.loadFrustum( transpose(mViewProjection) );

	std::vector< SceneMesh* >	oSceneMeshes;
	std::vector< SceneMesh* >	oTransparentSceneMeshes;
	std::vector< OmniLight* >	oOmniLights;
	std::vector< SpotLight* >	oSpotLights;
	std::vector< SpotShadow* >	oSpotShadows;
	
	const SkyBox* pSkyBox = rRenderContext.GetSkyBox();
	
	//Frustum culling
	GetVisibleObjects( rRenderContext, oViewFrustum, mView, oSceneMeshes, oTransparentSceneMeshes, oOmniLights, oSpotLights, oSpotShadows );

	if(!rRenderContext.GetDirectionalLights().empty())
	{
		if( !m_pDirectionalShadowLight )
		{
			m_pDirectionalShadowLight = rRenderContext.GetDirectionalLights().front();
		}
		if( bComputeFrustumBoundingSpheres )
		{
			ComputeFrustumBoundingSpheres( transpose(mInvProjection), rCamera.GetNear(), rCamera.GetFar() );
		}
		RenderCascadedShadowMap(rRenderContext.GetSceneMeshes(), !mView);
	}

	RenderShadowMaps( rRenderContext.GetSceneMeshes(),oSpotShadows,rHardwareRenderContext );

	rHardwareRenderContext.Reshape( iWindowWidth, iWindowHeight, rCamera.GetFOV(), rCamera.GetNear(), rCamera.GetFar() );	

	rRenderContext.PushMVP(mViewProjection);
	rRenderContext.PushModelView(mView);
	//GBuffer pass
	m_pGBuffer->Activate();
	
	//Render view-space normal and depth in 2 buffers
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	std::vector< SceneMesh* >::const_iterator oMeshIt = oSceneMeshes.begin();
	while( oMeshIt != oSceneMeshes.end() )
	{
		(*oMeshIt)->Draw( EffectTechnique::E_RENDER_GBUFFER );
		++oMeshIt;
	}
	
	m_pGBuffer->Deactivate();
	
	//Lighting pass

	//creates one quad per omni light
	PrepareOmniLights( oOmniLights, rCamera, mView, mViewProjection );
	//creates one full screen quad per directional light
	PrepareDirectionalLights( rRenderContext.GetDirectionalLights(), rCamera, mView );
	//creates one quad per spot light
	PrepareSpotLights( oSpotLights, rCamera, mView, mViewProjection );

	//enable blending in order to add all the light contributions
	
	glEnable(GL_BLEND);
	glBlendFunc( GL_ONE, GL_ONE );
	glDisable( GL_DEPTH_TEST );

	glActiveTexture( GL_TEXTURE0 );
	m_pGBuffer->ActivateTexture(0);
	glActiveTexture( GL_TEXTURE1 );
	m_pGBuffer->ActivateDepthTexture();
	glActiveTexture( GL_TEXTURE0 );

	m_pLightBuffer->Activate();
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	Engine::GrabInstance().GrabRenderingContext().ReshapeGlOrtho( iWindowWidth, iWindowHeight );

	float4x4 oOrthoMatrix = orthoMatrix(0.0, static_cast<float>(iWindowWidth), 0, static_cast<float>(iWindowHeight),-0.2f,0.2f);
	rRenderContext.PushMVP(oOrthoMatrix);

	if( !m_vDirectionalLightQuads.empty() )
	{
		m_pDirectionalLightShader->Activate();
		m_pDirectionalLightShader->CommitStdUniforms();
		m_pDirectionalLightShader->setUniformMatrix4fv( m_iDirectionalLightShaderInvProjHandle, mInvProjection );
		
		glActiveTexture( GL_TEXTURE2 );
		m_pDirectionalShadowLight->ActivateDepthTexture();
		float pMatrices[ 16 * DirectionalLight::iCascadeCount ];
		for( unsigned int i = 0; i < DirectionalLight::iCascadeCount; ++i)
		{
			float * pMatrice = transpose(m_pDirectionalShadowLight->GetMatrix(i) * mInvViewProjection);
			for(unsigned int j = 0; j < 16; ++j)
			{
				pMatrices[i*16+j] = pMatrice[j];
			}
		}

		m_pDirectionalLightShader->setUniformMatrix4fv( m_iDirectionalLightShaderShadowMatrixHandle, DirectionalLight::iCascadeCount, pMatrices );
		m_pDirectionalLightShader->setUniform4fv(m_iDirectionalLightShaderSphereHandle,DirectionalLight::iCascadeCount,m_pFrustumBoundingSpheres);
		RenderDirectionalLights( m_vDirectionalLightQuads );

		m_pDirectionalLightShader->Deactivate();
	}
	
	//Render Omni Lights
	if( !m_vOmniLightQuads.empty() )
	{
		m_pOmniLightShader->Activate();
		m_pOmniLightShader->CommitStdUniforms();
		m_pOmniLightShader->setUniformMatrix4fv( m_iOmniLightShaderInvProjHandle, mInvProjection );

		RenderOmniLights( m_vOmniLightQuads );
		
		m_pOmniLightShader->Deactivate();
	}
	
	//Render Spot Lights
	if( !m_vSpotLightQuads.empty() )
	{
		m_pSpotLightShader->Activate();
		m_pSpotLightShader->CommitStdUniforms();
		m_pSpotLightShader->setUniformMatrix4fv( m_iSpotLightShaderInvProjHandle, mInvProjection );
		RenderSpotLights( m_vSpotLightQuads, m_iSpotLightShaderColorAndInverseRadiusHandle, m_iSpotLightShaderViewSpacePosAndMultiplierHandle, m_iSpotLightShaderViewSpaceDirHandle, m_iSpotLightShaderCosInAndOutHandle );
		m_pSpotLightShader->Deactivate();
	}

	if( !oSpotShadows.empty() )
	{
		m_pSpotShadowShader->Activate();
		m_pSpotShadowShader->CommitStdUniforms();
		m_pSpotShadowShader->setUniformMatrix4fv( m_iSpotShadowShaderInvProjHandle, mInvProjection );

		glActiveTexture( GL_TEXTURE2 );
	
		PrepareAndRenderSpotShadows( oSpotShadows, rCamera, mView, mViewProjection );
		m_pSpotShadowShader->Deactivate();	

		Texture2D::Deactivate();
	}
	
	m_pLightBuffer->Deactivate();

	rRenderContext.PopMVP();

	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE1 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );

	glDisable(GL_BLEND);
	glEnable( GL_DEPTH_TEST );

	//Material pass
	//The material pass needs to fetch the light buffer
	glActiveTexture( GL_TEXTURE6 );
	m_pLightBuffer->ActivateTexture();
	
	//Restoring perspective view
	rHardwareRenderContext.Reshape( iWindowWidth, iWindowHeight, rCamera.GetFOV(), rCamera.GetNear(), rCamera.GetFar() );
	
	m_pHDRSceneBuffer->Activate();
	
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
	glDrawBuffers(2, buffers);
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//rendering opaque objects
	oMeshIt = oSceneMeshes.begin();	
	while( oMeshIt != oSceneMeshes.end() )
	{
		(*oMeshIt)->Draw( EffectTechnique::E_RENDER_OPAQUE );
		++oMeshIt;
	}
	
	if( pSkyBox )
	{
		pSkyBox->Draw( rCamera.GetPos(), m_iDebugFlag );
	}

	//rendering transparent objects
	
	glEnableIndexedEXT( GL_BLEND, 0 );
	glDepthMask( GL_FALSE );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	oMeshIt = oTransparentSceneMeshes.begin();
	glCullFace( GL_FRONT );
	while( oMeshIt != oTransparentSceneMeshes.end() )
	{
		(*oMeshIt)->Draw( EffectTechnique::E_RENDER_TRANSPARENCY );
		++oMeshIt;
	}

	oMeshIt = oTransparentSceneMeshes.begin();
	glCullFace( GL_BACK );
	while( oMeshIt != oTransparentSceneMeshes.end() )
	{
		(*oMeshIt)->Draw( EffectTechnique::E_RENDER_TRANSPARENCY );
		++oMeshIt;
	}

	
	//------------------ Draw particle
	ParticleRenderer& rParticleRenderer = rRenderContext.GrabParticleRenderer();
	if (rParticleRenderer.GrabBatchs().empty() == false)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		///Hack for now, retrive shader directly
		ShaderManager& rShaderManager = ShaderManager::GrabInstance();
		Shader* pParticleShader = rShaderManager.GetShader("../Data/Shaders/Material/xml/BasicParticle.bfx.xml");
	
		int iShaderHandle = pParticleShader->getHandle();
		int	iVertexHandle = glGetAttribLocation(iShaderHandle, "InVertex");
		int	iColorHandle = glGetAttribLocation(iShaderHandle, "InColor");
		int	iSizeHandle = glGetAttribLocation(iShaderHandle, "InSize");
		int	iUvHandle = glGetAttribLocation(iShaderHandle, "InTexCoord0");


		FOR_EACH_IT(Batchs, rParticleRenderer.GrabBatchs(), it)
		{
			ParticleBatch& rBatch = (*it);
			Material* pMaterial = rBatch.GrabMaterialPtr();
			//The size of particle, to make the jumps
			unsigned int iVertexSize = rBatch.GetParticleVertexSize();

			if (pMaterial->Activate(EffectTechnique::E_RENDER_TRANSPARENCY))
			{
				
				//Send Position
				glVertexAttribPointer(iVertexHandle, 3, GL_FLOAT, GL_FALSE, iVertexSize, rBatch.GetVertexBuffer());
				glVertexAttribPointer(iColorHandle, 4, GL_FLOAT, GL_FALSE, iVertexSize,  rBatch.GetVertexBuffer() + (sizeof(vec3)));
				glVertexAttribPointer(iSizeHandle, 2, GL_FLOAT, GL_FALSE, iVertexSize,  rBatch.GetVertexBuffer() + (sizeof(vec3) + sizeof(vec4)));
				glVertexAttribPointer(iUvHandle, 2, GL_FLOAT, GL_FALSE, iVertexSize,  rBatch.GetVertexBuffer() + (sizeof(vec3) + sizeof(vec4) + sizeof(vec2)));

				glEnableVertexAttribArray(iVertexHandle);
				glEnableVertexAttribArray(iColorHandle);
				glEnableVertexAttribArray(iSizeHandle);
				glEnableVertexAttribArray(iUvHandle);

				glDrawElements(GL_TRIANGLES, (GLsizei)rBatch.GetIndexCount(), GL_UNSIGNED_SHORT, rBatch.GetIndexBuffer());
				glDisableVertexAttribArray(iVertexHandle);
				glDisableVertexAttribArray(iColorHandle);
				glDisableVertexAttribArray(iSizeHandle);
				glDisableVertexAttribArray(iUvHandle);

			}
		}
	}
	//------------------ End particle
	
	glDepthMask( GL_TRUE );
	glDisableIndexedEXT( GL_BLEND, 0 );
	
	glDrawBuffers(1, buffers);

	glActiveTexture( GL_TEXTURE6 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Deactivate();
	m_pHDRSceneBuffer->Deactivate();
	rRenderContext.PopMVP();
	rRenderContext.PopModelView();
	ComputeAvgLum();
	
	//Tone mapping
	m_pToneMappingShader->Activate();
	rRenderContext.SetCurrentShader(m_pToneMappingShader);
	m_pToneMappingShader->setUniformf( m_iToneMappingShaderKeyHandle, m_fToneMappingKey );
	
	m_pHDRSceneBuffer->ActivateTexture();
	glActiveTexture( GL_TEXTURE1 );
	m_pLastAdaptationBuffer->ActivateTexture();

	m_pLDRSceneBuffer->Activate();
	DrawFullScreenQuad( iWindowWidth, iWindowHeight );
	m_pLDRSceneBuffer->Deactivate();

	m_pToneMappingShader->Deactivate();
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Deactivate();
	
	//Post process: bloom, color correction, fxaa
	m_pPostProcessShader->Activate();
	rRenderContext.SetCurrentShader(m_pPostProcessShader);

	m_pPostProcessShader->setUniformf( m_iPostProcessShaderGlowMultiplierHandle, m_fGlowMultiplier );
	m_pPostProcessShader->setUniformi( glGetUniformLocation( m_pPostProcessShader->getHandle(), "iDebug" ), m_iDebugFlag );
	m_pLDRSceneBuffer->ActivateTexture();

	glActiveTexture( GL_TEXTURE1 );
	m_pBrightPass1Buffer->ActivateTexture();

	glActiveTexture( GL_TEXTURE2 );
	m_pHDRSceneBuffer->ActivateTexture(1);

	glActiveTexture( GL_TEXTURE3 );
	m_pColorLUT->Activate();

	m_pLDRSceneBuffer2->Activate();
	DrawFullScreenQuad( iWindowWidth, iWindowHeight );
	m_pLDRSceneBuffer2->Deactivate();
	
	m_pPostProcessShader->Deactivate();
	
	Texture3D::Deactivate();
	glActiveTexture( GL_TEXTURE2 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE1 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Deactivate();

	//Downsampling scene for DOF
	m_pLDRSceneBuffer2->ActivateTexture();
	glActiveTexture( GL_TEXTURE1 );
	m_pHDRSceneBuffer->ActivateTexture(1);
	
	m_pDOFBlur1Buffer->Activate();
	m_pDownSample4x4DOF->Activate();
	rRenderContext.SetCurrentShader(m_pDownSample4x4DOF);
	float pPixelSize[2] = { 1.0f/ (iWindowWidth), 1.0f/ (iWindowHeight) };
	m_pDownSample4x4DOF->setUniform2fv( m_iDownSample4x4DOFPixelSizeHandle, 1, pPixelSize );
	DrawFullScreenQuad( iWindowWidth/2, iWindowHeight/2);
	
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );
	m_pDownSample4x4DOF->Deactivate();
	m_pDOFBlur1Buffer->Deactivate();

	//Gaussian Blur
	m_pDOFBlur1Buffer->ActivateTexture();
	m_pDOFBlur2Buffer->Activate();

	m_pBlur6Shader->Activate();
	rRenderContext.SetCurrentShader(m_pBlur6Shader);
	pPixelSize[0] = 1.0f / ( iWindowWidth / 2 );
	pPixelSize[1] = 0.0f;
	m_pBlur6Shader->setUniform2fv( m_iBlur6ShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( iWindowWidth / 2, iWindowHeight / 2 );

	m_pDOFBlur2Buffer->ActivateTexture();
	m_pDOFBlur1Buffer->Activate();
	pPixelSize[0] = 0.0f;
	pPixelSize[1] = 1.0f / ( iWindowHeight / 2 );
	m_pBlur6Shader->setUniform2fv( m_iBlur6ShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( iWindowWidth / 2, iWindowHeight / 2 );
	
	//Depth of field
	m_pDOFBlur1Buffer->Deactivate();
	Texture2D::Deactivate();
	
	m_pDOFShader->Activate();
	rRenderContext.SetCurrentShader(m_pDOFShader);
	glActiveTexture( GL_TEXTURE0 );
	m_pLDRSceneBuffer2->ActivateTexture();
	glActiveTexture( GL_TEXTURE1 );
	m_pDOFBlur1Buffer->ActivateTexture();

	DrawFullScreenQuad( iWindowWidth, iWindowHeight );

	Texture2D::Deactivate();

	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Deactivate();

	m_pDOFShader->Deactivate();
	
	DebugRender( oSceneMeshes, oTransparentSceneMeshes, oSpotShadows, oSpotLights );
	
	++m_fFrameCount;
	if( m_fFrameCount >= MAX_FRAME )
	{
		m_fFrameTime = m_oTimer->Stop() / MAX_FRAME;
		m_fFrameCount = 0.0;
		m_oTimer->Start();
	}
	if( m_bShowDebugMenu )	
	{
		//Profiling infos
		std::stringstream oGPUStream;
		oGPUStream << "GPU:" << m_fFrameTime << "ms";
		DisplayText( oGPUStream.str(), iWindowWidth - PROFILING_LEFT_OFFSET, 50, m_pFont );
		
		std::stringstream oOmniStream;
		oOmniStream << "Displaying " << oOmniLights.size() << " of " << m_iOmniCount << " omni light(s).";
		DisplayText( oOmniStream.str(), iWindowWidth - PROFILING_LEFT_OFFSET, 70, m_pFont );

		std::stringstream oSpotStream;
		oSpotStream << "Displaying " << oSpotLights.size() << " of " << m_iSpotCount << " spot light(s).";
		DisplayText( oSpotStream.str(), iWindowWidth - PROFILING_LEFT_OFFSET, 90, m_pFont );

		std::stringstream oSpotShadowStream;
		oSpotShadowStream << "Displaying " << oSpotShadows.size() << " of " << m_iSpotShadowCount << " spot shadow(s).";
		DisplayText( oSpotShadowStream.str(), iWindowWidth - PROFILING_LEFT_OFFSET, 110, m_pFont );

		std::stringstream oDirectionalStream;
		oDirectionalStream << "Displaying " << m_vDirectionalLightQuads.size() << " of " << m_iDirectionalCount << " directional light(s).";
		DisplayText( oDirectionalStream.str(), iWindowWidth - PROFILING_LEFT_OFFSET, 130, m_pFont );

		std::stringstream o3DObjectStream;
		o3DObjectStream << "Displaying " << oSceneMeshes.size() + oTransparentSceneMeshes.size() << " of " << m_iObjectCount << " 3D object(s).";
		DisplayText( o3DObjectStream.str(), iWindowWidth - PROFILING_LEFT_OFFSET, 170, m_pFont );
		
		DisplayDebugMenu();
	}
}

void DeferredRenderer::DebugRender( const std::vector< SceneMesh* >& oSceneMeshes, const std::vector< SceneMesh* >& oTransparentSceneMeshes, const std::vector< SpotShadow* >& oSpotShadows, const std::vector< SpotLight* >& oSpotLights )
{
	Engine& rEngine = Engine::GrabInstance();
	RenderingContext& rRenderContext = rEngine.GrabRenderContext();
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();
	
	OpenGLContext& rHardwareRenderContext = rEngine.GrabRenderingContext();
	AbstractCamera & rCamera = rEngine.GetCurrentCamera();
	
	rHardwareRenderContext.Reshape( iWindowWidth, iWindowHeight, rCamera.GetFOV(), rCamera.GetNear(), rCamera.GetFar() );

	glClear( GL_DEPTH_BUFFER_BIT );
	//DrawCube(m_pFrustumPoints);
	glColor3f(1.0f,0.0f,0.0f);
	if( m_iDebugBoundingBox )
	{
		std::vector< SceneMesh* >::const_iterator oMeshIt = oSceneMeshes.begin();	
		while( oMeshIt != oSceneMeshes.end() )
		{
			const float* pMeshBoundingBox = (*oMeshIt)->GetBoundingBox();

			vec3 pPoints[8];
			pPoints[0] = vec3( pMeshBoundingBox[0], pMeshBoundingBox[3], pMeshBoundingBox[4]  ); // Near top left
			pPoints[1] = vec3( pMeshBoundingBox[1], pMeshBoundingBox[3], pMeshBoundingBox[4]  ); // Near top right
			pPoints[2] = vec3( pMeshBoundingBox[1], pMeshBoundingBox[2], pMeshBoundingBox[4]  ); // Near bottom right
			pPoints[3] = vec3( pMeshBoundingBox[0], pMeshBoundingBox[2], pMeshBoundingBox[4]  ); // Near bottom left

			pPoints[4] = vec3( pMeshBoundingBox[0], pMeshBoundingBox[3], pMeshBoundingBox[5]  ); // Far top left
			pPoints[5] = vec3( pMeshBoundingBox[1], pMeshBoundingBox[3], pMeshBoundingBox[5]  ); // Far top right
			pPoints[6] = vec3( pMeshBoundingBox[1], pMeshBoundingBox[2], pMeshBoundingBox[5]  ); // Far bottom right
			pPoints[7] = vec3( pMeshBoundingBox[0], pMeshBoundingBox[2], pMeshBoundingBox[5]  ); // Far bottom left

			DrawCube( pPoints );

			++oMeshIt;
		}

		oMeshIt = oTransparentSceneMeshes.begin();	
		while( oMeshIt != oTransparentSceneMeshes.end() )
		{
			const float* pMeshBoundingBox = (*oMeshIt)->GetBoundingBox();

			vec3 pPoints[8];
			pPoints[0] = vec3( pMeshBoundingBox[0], pMeshBoundingBox[3], pMeshBoundingBox[4]  ); // Near top left
			pPoints[1] = vec3( pMeshBoundingBox[1], pMeshBoundingBox[3], pMeshBoundingBox[4]  ); // Near top right
			pPoints[2] = vec3( pMeshBoundingBox[1], pMeshBoundingBox[2], pMeshBoundingBox[4]  ); // Near bottom right
			pPoints[3] = vec3( pMeshBoundingBox[0], pMeshBoundingBox[2], pMeshBoundingBox[4]  ); // Near bottom left

			pPoints[4] = vec3( pMeshBoundingBox[0], pMeshBoundingBox[3], pMeshBoundingBox[5]  ); // Far top left
			pPoints[5] = vec3( pMeshBoundingBox[1], pMeshBoundingBox[3], pMeshBoundingBox[5]  ); // Far top right
			pPoints[6] = vec3( pMeshBoundingBox[1], pMeshBoundingBox[2], pMeshBoundingBox[5]  ); // Far bottom right
			pPoints[7] = vec3( pMeshBoundingBox[0], pMeshBoundingBox[2], pMeshBoundingBox[5]  ); // Far bottom left

			DrawCube( pPoints );

			++oMeshIt;
		}
	}
	if( m_iDebugSpotLightFrustum )
	{
		std::vector< SpotShadow*>::const_iterator oShadowIt = oSpotShadows.begin();
		std::vector< SpotShadow*>::const_iterator oShadowEnd = oSpotShadows.end();
		while( oShadowIt != oShadowEnd )
		{
			const float* pBoundingBox = (*oShadowIt)->GetBoundingBox();

			vec3 pPoints[8];
			pPoints[0] = vec3( pBoundingBox[0], pBoundingBox[3], pBoundingBox[4]  ); // Near top left
			pPoints[1] = vec3( pBoundingBox[1], pBoundingBox[3], pBoundingBox[4]  ); // Near top right
			pPoints[2] = vec3( pBoundingBox[1], pBoundingBox[2], pBoundingBox[4]  ); // Near bottom right
			pPoints[3] = vec3( pBoundingBox[0], pBoundingBox[2], pBoundingBox[4]  ); // Near bottom left

			pPoints[4] = vec3( pBoundingBox[0], pBoundingBox[3], pBoundingBox[5]  ); // Far top left
			pPoints[5] = vec3( pBoundingBox[1], pBoundingBox[3], pBoundingBox[5]  ); // Far top right
			pPoints[6] = vec3( pBoundingBox[1], pBoundingBox[2], pBoundingBox[5]  ); // Far bottom right
			pPoints[7] = vec3( pBoundingBox[0], pBoundingBox[2], pBoundingBox[5]  ); // Far bottom left

			DrawCube( pPoints );

			DrawFrustum( (*oShadowIt)->GetFarPlanePoints() , (*oShadowIt)->GetPos() );

			oShadowIt++;
		}

		std::vector< SpotLight*>::const_iterator oSpotIt = oSpotLights.begin();
		std::vector< SpotLight*>::const_iterator oSpotEnd = oSpotLights.end();
		while( oSpotIt != oSpotEnd )
		{
			const float* pBoundingBox = (*oSpotIt)->GetBoundingBox();

			vec3 pPoints[8];
			pPoints[0] = vec3( pBoundingBox[0], pBoundingBox[3], pBoundingBox[4]  ); // Near top left
			pPoints[1] = vec3( pBoundingBox[1], pBoundingBox[3], pBoundingBox[4]  ); // Near top right
			pPoints[2] = vec3( pBoundingBox[1], pBoundingBox[2], pBoundingBox[4]  ); // Near bottom right
			pPoints[3] = vec3( pBoundingBox[0], pBoundingBox[2], pBoundingBox[4]  ); // Near bottom left

			pPoints[4] = vec3( pBoundingBox[0], pBoundingBox[3], pBoundingBox[5]  ); // Far top left
			pPoints[5] = vec3( pBoundingBox[1], pBoundingBox[3], pBoundingBox[5]  ); // Far top right
			pPoints[6] = vec3( pBoundingBox[1], pBoundingBox[2], pBoundingBox[5]  ); // Far bottom right
			pPoints[7] = vec3( pBoundingBox[0], pBoundingBox[2], pBoundingBox[5]  ); // Far bottom left

			DrawCube( pPoints );

			DrawFrustum( (*oSpotIt)->GetFarPlanePoints() , (*oSpotIt)->GetPos() );

			oSpotIt++;
		}
	}

	if( m_iDebugRender == 1 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_pGBuffer->ActivateTexture();
		m_pBasicTextureShader->Activate();
		rRenderContext.SetCurrentShader(m_pBasicTextureShader);
		DrawFullScreenQuad( iWindowWidth, iWindowHeight );
		m_pBasicTextureShader->Deactivate();
	}
	else if( m_iDebugRender == 2 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_pGBuffer->ActivateTexture();
		m_pDebugGlossShader->Activate();
		rRenderContext.SetCurrentShader(m_pDebugGlossShader);
		DrawFullScreenQuad( iWindowWidth, iWindowHeight );
		m_pDebugGlossShader->Deactivate();
	}
	else if( m_iDebugRender == 3 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_pLightBuffer->ActivateTexture();
		m_pBasicTextureShader->Activate();
		rRenderContext.SetCurrentShader(m_pBasicTextureShader);
		DrawFullScreenQuad( iWindowWidth, iWindowHeight );
		m_pBasicTextureShader->Deactivate();
	}
	else if( m_iDebugRender == 4 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_pLightBuffer->ActivateTexture();
		m_pDebugSpecularShader->Activate();
		rRenderContext.SetCurrentShader(m_pDebugSpecularShader);
		DrawFullScreenQuad( iWindowWidth, iWindowHeight );
		m_pDebugSpecularShader->Deactivate();
	}
	else if( m_iDebugRender == 5 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_pHDRSceneBuffer->ActivateTexture();
		m_pBasicTextureShader->Activate();
		rRenderContext.SetCurrentShader(m_pBasicTextureShader);
		DrawFullScreenQuad( iWindowWidth, iWindowHeight );
		m_pBasicTextureShader->Deactivate();
	}
	else if( m_iDebugRender == 6 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_pBrightPass1Buffer->ActivateTexture();
		m_pBasicTextureShader->Activate();
		rRenderContext.SetCurrentShader(m_pBasicTextureShader);
		DrawFullScreenQuad( iWindowWidth, iWindowHeight );
		m_pBasicTextureShader->Deactivate();
	}
	else if( m_iDebugRender == 7 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_pGBuffer->ActivateDepthTexture();
		m_pDebugDepthShader->Activate();
		rRenderContext.SetCurrentShader(m_pDebugDepthShader);
		DrawFullScreenQuad( iWindowWidth, iWindowHeight );
		m_pDebugDepthShader->Deactivate();
	}
	else if( m_iDebugRender == 8 && m_pDirectionalShadowLight )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_pDirectionalShadowLight->ActivateDepthTexture();
		m_pBasicTextureShader->Activate();
		rRenderContext.SetCurrentShader(m_pBasicTextureShader);
		DrawFullScreenQuad( iWindowWidth, iWindowHeight );
		m_pBasicTextureShader->Deactivate();
	}
}