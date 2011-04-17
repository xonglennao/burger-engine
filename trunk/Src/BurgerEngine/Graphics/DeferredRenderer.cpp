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

#include "BurgerEngine/Core/Timer.h"

#include "BurgerEngine/External/Math/Vector.h"
#include "BurgerEngine/External/Math/Frustum.h"
#include "BurgerEngine/External/GLFont/glfont.h"

#include <sstream>
#include <algorithm>

#include "BurgerEngine/Graphics/StaticMesh.h"

#include "BurgerEngine/Graphics/SkyBox.h"

#include "BurgerEngine/Graphics/TextureManager.h"

//#define NO_MATERIAL_PASS

const int GLOW_RATIO = 4;
const float MAX_FRAME = 15.0;

const int PROFILING_LEFT_OFFSET = 300;

float fInc = 0.0f;
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
DeferredRenderer::DeferredRenderer()
	: m_iDebugFlag(0)
	, m_iSkipCulling(0)
	, m_iDebugBoundingBox(0)
	, m_iDebugSpotLightFrustum(0)
	, m_fFrameTime( 10.0 )
	, m_bShowDebugMenu( false )
	, m_fToneMappingKey( 0.5f )
	, m_fGlowMultiplier( 1.0f )
	, m_fBrightPassThreshold( 0.5f )
	, m_fBrightPassOffset( 10.0f )
	, m_fAdaptationBaseTime( 0.6f )
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

	GenFullScreenQuad();

	DebugMenu& oDebugMenu = Engine::GrabInstance().GrabRenderContext().GetDebugMenu();

	oDebugMenu.AddEntry( "ToneMappingKey", m_fToneMappingKey, 0.0f, 1.0f, 0.1f );
	oDebugMenu.AddEntry( "Glow Multiplier", m_fGlowMultiplier, 0.0f, 10.0f, 0.5f );
	oDebugMenu.AddEntry( "BrightPass Threshold", m_fBrightPassThreshold, 0.0f, 10.0f, 0.5f );
	oDebugMenu.AddEntry( "BrightPass Offset", m_fBrightPassOffset, 0.0f, 10.0f, 0.5f );
	oDebugMenu.AddEntry( "Adaptation Base Time", m_fAdaptationBaseTime, 0.0f, 1.0f, 0.02f );
	oDebugMenu.AddEntry( "DebugFlag", m_iDebugFlag, 0, 100, 1 );
	oDebugMenu.AddEntry( "Skip Culling", m_iSkipCulling, 0, 1, 1 );
	oDebugMenu.AddEntry( "Show Bounding Boxes", m_iDebugBoundingBox, 0, 1, 1 );
	oDebugMenu.AddEntry( "Show Spot Lights Frustum", m_iDebugSpotLightFrustum, 0, 1, 1 );

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
	//m_pHDRSceneBuffer->GenerateColorOnly( GL_RGBA16F_ARB );
	m_pHDRSceneBuffer->GenerateFinalHDRBuffer();

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

	m_pLastAdaptationBuffer = new FBO( 1, 1, FBO::E_FBO_2D );
	m_pLastAdaptationBuffer->GenerateColorOnly( GL_INTENSITY16F_ARB );

	m_pBrightPass1Buffer = new FBO( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO, FBO::E_FBO_2D );
	m_pBrightPass1Buffer->GenerateColorOnly();

	m_pBrightPass2Buffer = new FBO( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO, FBO::E_FBO_2D );
	m_pBrightPass2Buffer->GenerateColorOnly();

	m_pDOFBlur1Buffer = new FBO( iWindowWidth/2, iWindowHeight/2, FBO::E_FBO_2D );
	m_pDOFBlur1Buffer->GenerateColorOnly( GL_RGBA16F_ARB );

	m_pDOFBlur2Buffer = new FBO( iWindowWidth/2, iWindowHeight/2, FBO::E_FBO_2D );
	m_pDOFBlur2Buffer->GenerateColorOnly( GL_RGBA16F_ARB );
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
	
	m_iDirectionalLightShaderColor = glGetAttribLocation( m_pDirectionalLightShader->getHandle(),"vColor");
	m_iDirectionalLightShaderViewSpacePosAndMultiplierHandle = glGetAttribLocation( m_pDirectionalLightShader->getHandle(),"vViewSpacePosAndMultiplier");

	m_pDirectionalLightShader->setUniformTexture("sNormalSampler",0);
	m_pDirectionalLightShader->setUniformTexture("sDepthSampler",1);

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

	//Blur 6 shader
	m_pBlur6Shader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/GaussianBlur6.bfx.xml" );
	m_pBlur6Shader->Activate();
	m_iBlur6ShaderPixelSizeHandle = glGetUniformLocation( m_pBlur6Shader->getHandle(), "vPixelSize" );
	m_pBlur6Shader->setUniformTexture("sTexture",0);
	m_pBlur6Shader->Deactivate();

	//Blur 6 used with multiple render targets
	m_pBlur6DofSpecialShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/GaussianBlur6DofSpecial.bfx.xml" );
	m_pBlur6DofSpecialShader->Activate();
	m_iBlur6DofSpecialShaderPixelSizeHandle = glGetUniformLocation( m_pBlur6DofSpecialShader->getHandle(), "vPixelSize" );
	m_pBlur6DofSpecialShader->setUniformTexture("sTexture",0);
	m_pBlur6DofSpecialShader->setUniformTexture("sBlurData",1);
	m_pBlur6DofSpecialShader->Deactivate();

	//Blur 10 shader
	m_pBlur10Shader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/GaussianBlur10.bfx.xml" );
	m_pBlur10Shader->Activate();
	m_iBlur10ShaderPixelSizeHandle = glGetUniformLocation( m_pBlur10Shader->getHandle(), "vPixelSize" );
	m_pBlur10Shader->setUniformTexture("sTexture",0);
	m_pBlur10Shader->Deactivate();

	//Log space Blur 10 shader
	m_pLogBlur10Shader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/LogGaussianBlur10.bfx.xml" );
	m_pLogBlur10Shader->Activate();
	m_iLogBlur10ShaderPixelSizeHandle = glGetUniformLocation( m_pLogBlur10Shader->getHandle(), "vPixelSize" );
	m_pLogBlur10Shader->setUniformTexture("sTexture",0);
	m_pLogBlur10Shader->Deactivate();

	//Downsample shader
	m_pDownSample4x4Shader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/DownSample4x4.bfx.xml" );
	m_pDownSample4x4Shader->Activate();
	m_pDownSample4x4Shader->setUniformTexture("sTexture",0);
	m_iDownSampleShaderPixelSizeHandle = glGetUniformLocation( m_pDownSample4x4Shader->getHandle(), "vPixelSize" );
	m_pDownSample4x4Shader->Deactivate();

	//Average Luminance shaders
	m_pAvgLumInitShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/AvgLumInit.bfx.xml" );
	m_pAvgLumInitShader->Activate();
	m_pAvgLumInitShader->setUniformTexture("sTexture",0);
	m_iAvgLumInitShaderPixelSizeHandle = glGetUniformLocation( m_pAvgLumInitShader->getHandle(), "vPixelSize" );
	m_pAvgLumInitShader->Deactivate();

	m_pAvgLumFinalShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/AvgLumFinal.bfx.xml" );
	m_pAvgLumFinalShader->Activate();
	m_pAvgLumFinalShader->setUniformTexture("sTexture",0);
	m_iAvgLumFinalShaderPixelSizeHandle = glGetUniformLocation( m_pAvgLumFinalShader->getHandle(), "vPixelSize" );
	m_pAvgLumFinalShader->Deactivate();

	//Post process shaders
	m_pToneMappingShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/ToneMapping.bfx.xml" );
	m_pToneMappingShader->Activate();
	m_pToneMappingShader->QueryStdUniforms();
	m_pToneMappingShader->setUniformTexture("sTexture",0);
	m_pToneMappingShader->setUniformTexture("sLuminance",1);
	m_pToneMappingShader->setUniformTexture("sBloom",2);
	m_pToneMappingShader->setUniformTexture("sDownSampledTexture",3);
	m_pToneMappingShader->setUniformTexture("sBlurData",4);
	m_pToneMappingShader->setUniformTexture("sLUT",5);
	m_iToneMappingShaderKeyAndMultiplierHandle = glGetUniformLocation( m_pToneMappingShader->getHandle(), "fGlowMultiplierAndKey" );
	m_pToneMappingShader->Deactivate();

	m_pLightAdaptationShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/LightAdaptation.bfx.xml" );
	m_pLightAdaptationShader->Activate();
	m_pLightAdaptationShader->QueryStdUniforms();
	m_pLightAdaptationShader->setUniformTexture("sAvgLuminance",0);
	m_pLightAdaptationShader->setUniformTexture("sAdaptedLuminance",1);
	m_iAdaptationShaderTimeHandle = glGetUniformLocation( m_pLightAdaptationShader->getHandle(), "fElapsedAndBaseTime" );
	m_pLightAdaptationShader->Deactivate();

	m_pBrightPassShader = rShaderManager.AddShader( "../Data/Shaders/Engine/xml/BrightPass.bfx.xml" );
	m_pBrightPassShader->Activate();
	m_iBrightPassShaderInvViewPortHandle = glGetUniformLocation( m_pBrightPassShader->getHandle(), "vInvViewport" );
	m_iBrightPassShaderThresholdOffsetKeyHandle = glGetUniformLocation( m_pBrightPassShader->getHandle(), "fThresholdOffSetKey" );
	m_pBrightPassShader->setUniformTexture("sTexture",0);
	m_pBrightPassShader->setUniformTexture("sLuminance",1);	
	m_pBrightPassShader->Deactivate();


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
	Engine::GrabInstance().GrabRenderingContext().ReshapeGlOrtho( iWindowWidth, iWindowHeight );
	
	glPushMatrix();

	glScalef( static_cast<float>(iWindowWidth), static_cast<float>(iWindowHeight), 0.0f );
	
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

	glPopMatrix();
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
	glColor3f(1.0f,0.0f,0.0f);
	glDisable( GL_DEPTH_TEST );

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
	
	glEnable( GL_DEPTH_TEST );
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
void DeferredRenderer::PrepareDirectionalLights( std::vector< SceneLight* > const& oSceneLights, AbstractCamera const& rCamera, float4x4 const& mView )
{
	m_vDirectionalLightQuads.clear();
	std::vector< SceneLight* >::const_iterator oLightIt = oSceneLights.begin();
	std::vector< SceneLight* >::const_iterator oLightItEnd = oSceneLights.end();

	while( oLightIt != oLightItEnd )
	{
		SceneLight * pLight = (*oLightIt);
		
		vec3 f3Pos = pLight->GetPos();

		SceneLight::SceneLightQuad oQuad;

		//computing view space position
		vec4 vViewSpacePos = mView * vec4( f3Pos.x, f3Pos.y, f3Pos.z, 0.0 );
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
		mat4 mShadowMatrix = !transpose(mViewProjection) * (*oLightIt)->GetMatrix();

		m_pSpotShadowShader->setUniformMatrix4fv( m_iSpotShadowShaderShadowMatrixHandle, mShadowMatrix );

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
void DeferredRenderer::RenderShadowMaps( const std::vector< SceneMesh* >& oSceneMeshes, const std::vector< SpotShadow* >& oSpotShadows, OpenGLContext& a_rDriverRenderingContext )
{
	if( m_iDebugFlag == 0 )
	{
		glCullFace( GL_BACK );
	}
	else
	{
		glCullFace( GL_FRONT );
	}

	std::vector< SpotShadow* >::const_iterator oSpotIt = oSpotShadows.begin();
	std::vector< SpotShadow* >::const_iterator oEnd = oSpotShadows.end();

	while( oSpotIt != oEnd )
	{
		SpotShadow * pSpot = *oSpotIt;
		
		vec3 oLightPos = pSpot->GetPos();

		///////////////
		
		//fInc += 0.001f;
		//pSpot->SetPos( vec3( sinf( fInc ) * 40.0f, oLightPos.y, oLightPos.z) );
		//pSpot->ComputeBoundingBox();
		
		//////////////

		vec3 oLightRotation = pSpot->GetRotation();

		float fRadius = pSpot->GetRadius();

		a_rDriverRenderingContext.Reshape( SpotShadow::iShadowMapSize, SpotShadow::iShadowMapSize, 2.0f * acosf( pSpot->GetCosOuterAngle()  ) / (float)M_PI * 180.0f, 0.01f, fRadius );
		glRotatef( -oLightRotation.x, 1.0,0.0,0.0 );
		glRotatef( oLightRotation.y, 0.0,1.0,0.0 );

		glTranslatef( -oLightPos.x, -oLightPos.y, -oLightPos.z );

		float4x4 mLightProjection = GlperspectiveMatrixY( 2.0f * acosf( pSpot->GetCosOuterAngle()  ) * RAD_TO_DEG, 1.0f,0.5, fRadius );

		float4x4 mLightView =  rotateXY( -oLightRotation.x*DEG_TO_RAD, -oLightRotation.y*DEG_TO_RAD ) * translate( -oLightPos.x, -oLightPos.y, -oLightPos.z );
		
		pSpot->SetMatrix( transpose( mLightProjection * mLightView ) );
		
		float4x4 mLightViewProjection = transpose(mLightProjection) * mLightView;

		Frustum oViewFrustum;
		oViewFrustum.loadFrustum( transpose(mLightViewProjection) );

		m_pExponentialShadowMapShader->Activate();
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

		m_pExponentialShadowMapShader->Deactivate();
		
		pSpot->DeactivateBuffer();
		
		pSpot->ActivateDepthTexture();
			
		m_pLogBlur10Shader->Activate();
		float pPixelSize[2] = { 1.0f/ SpotShadow::iShadowMapSize, 0.0f };
		m_pLogBlur10Shader->setUniform2fv( m_iLogBlur10ShaderPixelSizeHandle, 1, pPixelSize );

		m_pSpotShadowBlurBuffer->Activate();
		DrawFullScreenQuad( SpotShadow::iShadowMapSize, SpotShadow::iShadowMapSize, m_iDebugFlag == 0 );

		m_pSpotShadowBlurBuffer->Deactivate();
		
		m_pSpotShadowBlurBuffer->ActivateTexture();

		pPixelSize[0] = 0.0f;
		pPixelSize[1] = 1.0f/ SpotShadow::iShadowMapSize;
		m_pLogBlur10Shader->setUniform2fv( m_iLogBlur10ShaderPixelSizeHandle, 1, pPixelSize);
			
		pSpot->ActivateBuffer();
		DrawFullScreenQuad( SpotShadow::iShadowMapSize, SpotShadow::iShadowMapSize, m_iDebugFlag == 0 );

		pSpot->DeactivateBuffer();

		m_pLogBlur10Shader->Deactivate();
		
		++oSpotIt;
	}

	glCullFace( GL_BACK );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::ComputeAvgLum()
{
	Engine const& rEngine = Engine::GetInstance();
	
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();
	
	//Downsampling HDR Scene
	m_pDownSampledSceneBuffer->Activate();

	glActiveTexture( GL_TEXTURE0 );
	m_pHDRSceneBuffer->ActivateTexture();
	
	m_pDownSample4x4Shader->Activate();
	
	float pPixelSize[2] = { 1.0f/ ( iWindowWidth/GLOW_RATIO ), 1.0f / ( iWindowHeight/GLOW_RATIO ) };
	m_pDownSample4x4Shader->setUniform2fv( m_iDownSampleShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( iWindowWidth/GLOW_RATIO, iWindowHeight/GLOW_RATIO );	
	
	m_pDownSample4x4Shader->Deactivate();

	m_pDownSampledSceneBuffer->Deactivate();

	//Computing average luminance
	m_p64x64LumBuffer->Activate();
	m_pAvgLumInitShader->Activate();
	
	m_pDownSampledSceneBuffer->ActivateTexture();
	
	pPixelSize[0] = 1.0f/ 64.0f;
	pPixelSize[1] = 1.0f / 64.0f;
	m_pAvgLumInitShader->setUniform2fv( m_iAvgLumInitShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( 64, 64 );
	m_p64x64LumBuffer->Deactivate();
	
	//Downsampling to 16x16
	m_p16x16LumBuffer->Activate();
	m_pDownSample4x4Shader->Activate();
	
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
	
	m_p4x4LumBuffer->ActivateTexture();
	
	pPixelSize[0] = 1.0f;
	pPixelSize[1] = 1.0f;
	m_pAvgLumFinalShader->setUniform2fv( m_iAvgLumFinalShaderPixelSizeHandle, 1, pPixelSize);
	DrawFullScreenQuad( 1, 1 );
	m_p1x1LumBuffer->Deactivate();
	
	//Light Adaptation
	m_pCurrentAdaptationBuffer->Activate();
	m_pLightAdaptationShader->Activate();
	
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
	float4x4 mProjection = GlperspectiveMatrixY( rCamera.GetFOV(), (float)iWindowWidth/(float)iWindowHeight,rCamera.GetNear(), rCamera.GetFar() );
	float4x4 mInvProjection = !mProjection;

	vec3 f3CamPos = rCamera.GetPos();
	float4x4 mView =  rCamera.GetViewMatrix();//rotateXY( -rCamera.GetRX()*DEG_TO_RAD, rCamera.GetRY()*DEG_TO_RAD ) * translate( -f3CamPos.x, -f3CamPos.y, -f3CamPos.z );

	float4x4 mViewProjection = transpose(mProjection) * mView;

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

	RenderShadowMaps( rRenderContext.GetSceneMeshes(),oSpotShadows,rHardwareRenderContext );
	
	rHardwareRenderContext.Reshape( iWindowWidth, iWindowHeight, rCamera.GetFOV(), rCamera.GetNear(), rCamera.GetFar() );	
	rCamera.LookAt();

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

#ifndef NO_MATERIAL_PASS
	m_pLightBuffer->Activate();
#endif
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	Engine::GrabInstance().GrabRenderingContext().ReshapeGlOrtho( iWindowWidth, iWindowHeight );

	if( !m_vDirectionalLightQuads.empty() )
	{
		m_pDirectionalLightShader->Activate();	
		m_pDirectionalLightShader->CommitStdUniforms();
		m_pDirectionalLightShader->setUniformMatrix4fv( m_iDirectionalLightShaderInvProjHandle, mInvProjection );		
		
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
#ifndef NO_MATERIAL_PASS
	m_pLightBuffer->Deactivate();
#endif

	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE1 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );

	glDisable(GL_BLEND);
	glEnable( GL_DEPTH_TEST );

#ifndef NO_MATERIAL_PASS	
	//Material pass
	//The material pass needs to fetch the light buffer
	glActiveTexture( GL_TEXTURE6 );
	m_pLightBuffer->ActivateTexture();
	
	//Restoring perspective view
	rHardwareRenderContext.Reshape( iWindowWidth, iWindowHeight, rCamera.GetFOV(), rCamera.GetNear(), rCamera.GetFar() );
	rCamera.LookAt();
	
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
	
	glDepthMask( GL_TRUE );
	glDisableIndexedEXT( GL_BLEND, 0 );

	glDrawBuffers(1, buffers);

	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Deactivate();
	m_pHDRSceneBuffer->Deactivate();

	ComputeAvgLum();
	
	//Downsampling scene for DOF
	m_pHDRSceneBuffer->ActivateTexture();
	glActiveTexture( GL_TEXTURE1 );
	m_pHDRSceneBuffer->ActivateTexture(1);
	
	m_pDOFBlur1Buffer->Activate();
	m_pBlur6DofSpecialShader->Activate();
	float pPixelSize[2] = { 1.0f/ (iWindowWidth/2), 0.0f };
	m_pBlur6DofSpecialShader->setUniform2fv( m_iBlur6DofSpecialShaderPixelSizeHandle, 1, pPixelSize );
	DrawFullScreenQuad( iWindowWidth/2, iWindowHeight/2);
	
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );

	m_pBlur6Shader->Activate();
	m_pDOFBlur2Buffer->Activate();
	m_pDOFBlur1Buffer->ActivateTexture();
	pPixelSize[0] = 0.0;
	pPixelSize[1] = 1.0f / (iWindowHeight/2);
	m_pBlur6Shader->setUniform2fv( m_iBlur6ShaderPixelSizeHandle, 1, pPixelSize );
	DrawFullScreenQuad( iWindowWidth/2, iWindowHeight/2);

	m_pDOFBlur2Buffer->Deactivate();

	//Tone mapping - Final Step
	m_pToneMappingShader->Activate();
	m_pToneMappingShader->CommitStdUniforms();
	
	float fToneMappingShaderUniform[2] = { m_fGlowMultiplier, m_fToneMappingKey };
	m_pToneMappingShader->setUniform2fv( m_iToneMappingShaderKeyAndMultiplierHandle, 1, fToneMappingShaderUniform );
	
	m_pHDRSceneBuffer->ActivateTexture();
	glActiveTexture( GL_TEXTURE1 );
	m_pLastAdaptationBuffer->ActivateTexture();

	glActiveTexture( GL_TEXTURE2 );
	m_pBrightPass1Buffer->ActivateTexture();

	glActiveTexture( GL_TEXTURE3 );
	m_pDOFBlur2Buffer->ActivateTexture();
	
	glActiveTexture( GL_TEXTURE4 );
	m_pHDRSceneBuffer->ActivateTexture(1);

	glActiveTexture( GL_TEXTURE5 );
	m_pColorLUT->Activate();

	DrawFullScreenQuad( iWindowWidth, iWindowHeight );
	m_pToneMappingShader->Deactivate();
	
	//Texture3D::Deactivate();
	glActiveTexture( GL_TEXTURE4 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE3 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE2 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE1 );
	Texture2D::Deactivate();
	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Deactivate();
#endif

	///////////////////////
	rHardwareRenderContext.Reshape( iWindowWidth, iWindowHeight, rCamera.GetFOV(), rCamera.GetNear(), rCamera.GetFar() );
	rCamera.LookAt();
	if( m_iDebugBoundingBox )
	{
		oMeshIt = oSceneMeshes.begin();	
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