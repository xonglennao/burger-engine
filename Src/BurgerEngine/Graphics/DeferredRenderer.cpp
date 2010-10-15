#include "BurgerEngine/Graphics/DeferredRenderer.h"

#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/SceneGraph.h"

#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Graphics/OpenGLContext.h"
#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/FBO.h"
#include "BurgerEngine/Graphics/ShaderManager.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/Texture2D.h"

#include "BurgerEngine/Core/Timer.h"

#include "BurgerEngine/External/Math/Vector.h"
#include "BurgerEngine/External/Math/Frustum.h"
#include "BurgerEngine/External/GLFont/glfont.h"

#include <sstream>

#include "BurgerEngine/Graphics/MeshManager.h"
#include "BurgerEngine/Graphics/StaticMesh.h"

//static float fAngle = 0.0f;

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
DeferredRenderer::DeferredRenderer()
	: m_iDebugFlag(0)
{
	Engine const& rEngine = Engine::GetInstance();

	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	m_oGBuffer = new FBO( iWindowWidth, iWindowHeight, FBO::E_FBO_2D );
	m_oGBuffer->Generate();

	m_oLightBuffer = new FBO( iWindowWidth, iWindowHeight, FBO::E_FBO_2D );
	m_oLightBuffer->GenerateColorOnly();

	//loading engine shaders

	m_pOmniLightShader = ShaderManager::GrabInstance().addShader( "OmniLightShader", "../Data/Shaders/OmniLight.vert", "../Data/Shaders/OmniLight.frag" );
	m_pOmniLightShader->Activate();

	m_pOmniLightShader->QueryStdUniforms();
	m_iOmniLightShaderInvProjHandle = glGetUniformLocation( m_pOmniLightShader->getHandle(), "mInvProj" );
	
	m_iOmniLightShaderColorAndInverseRadiusHandle = glGetAttribLocation( m_pOmniLightShader->getHandle(),"vColorAndInverseRadius");
	m_iOmniLightShaderViewSpacePosAndMultiplierHandle = glGetAttribLocation( m_pOmniLightShader->getHandle(),"vViewSpacePosAndMultiplier");

	m_pOmniLightShader->setUniformTexture("sNormalSampler",0);
	m_pOmniLightShader->setUniformTexture("sDepthSampler",1);

	m_pOmniLightShader->Desactivate();

	m_pSpotLightShader = ShaderManager::GrabInstance().addShader( "SpotLightShader", "../Data/Shaders/SpotLight.vert", "../Data/Shaders/SpotLight.frag" );
	m_pSpotLightShader->Activate();

	m_pSpotLightShader->QueryStdUniforms();
	m_iSpotLightShaderInvProjHandle = glGetUniformLocation( m_pSpotLightShader->getHandle(), "mInvProj" );
	m_iSpotLightShaderColorAndInverseRadiusHandle = glGetAttribLocation( m_pSpotLightShader->getHandle(),"vColorAndInverseRadius");
	m_iSpotLightShaderViewSpacePosAndMultiplierHandle = glGetAttribLocation( m_pSpotLightShader->getHandle(),"vViewSpacePosAndMultiplier");
	m_iSpotLightShaderViewSpaceDirHandle = glGetAttribLocation( m_pSpotLightShader->getHandle(),"vViewSpaceDir");
	m_iSpotLightShaderCosInAndOutHandle = glGetAttribLocation( m_pSpotLightShader->getHandle(),"vCosInAndOut");
	
	m_pSpotLightShader->setUniformTexture("sNormalSampler",0);
	m_pSpotLightShader->setUniformTexture("sDepthSampler",1);

	m_pSpotLightShader->Desactivate();

	m_pSpotShadowShader = ShaderManager::GrabInstance().addShader( "SpotShadowShader", "../Data/Shaders/SpotShadow.vert", "../Data/Shaders/SpotShadow.frag" );
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
	m_pSpotShadowShader->Desactivate();
	
	//loading font
	GLuint iFontId;
	glGenTextures(1, &iFontId);
	m_oFont = new PixelPerfectGLFont();
	m_oFont->Create("../Data/Fonts/lucida_sans.glf", iFontId);

	m_oTimer = new Timer();
	m_fMaxFrame = 15.0;
	m_fFrameCount = m_fMaxFrame;


}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
DeferredRenderer::~DeferredRenderer()
{
	delete m_oGBuffer;
	m_oGBuffer = NULL;

	delete m_oFont;
	m_oFont = NULL;

	delete m_oTimer;
	m_oTimer = NULL;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::DrawFullScreenQuad( int iWindowWidth, int iWindowHeight )
{
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,0,iWindowHeight,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	  
	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);
	
	glTexCoord2f(0.0f,0.0f);
	glVertex2i(0,0);
	
	glTexCoord2f(0.0f,1.0f);
	glVertex2i(0,iWindowHeight);
	
	glTexCoord2f(1.0f,1.0f);
	glVertex2i(iWindowWidth,iWindowHeight);
	
	glTexCoord2f(1.0f,0.0f);
	glVertex2i(iWindowWidth,0);
	glEnd();
	
	glEnable(GL_CULL_FACE);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::RenderOmniLights( std::vector< OmniLight::OmniLightQuad > vOmniLightQuads )
{
	Engine const& rEngine = Engine::GetInstance();
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,0,iWindowHeight,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	glDisable(GL_CULL_FACE);

	float fHalfWidth = iWindowWidth * 0.5f; 
	float fHalfHeight = iWindowHeight * 0.5f; 

	GLuint iVertexBufferId;	
	glGenBuffers(1,  &iVertexBufferId);

	unsigned int iNbVertex = vOmniLightQuads.size() * 4;
	unsigned int iSizeOfOmniVertex = sizeof(OmniLight::OmniLightVertex);

	unsigned int iSizeOmniVertex = ( iNbVertex * iSizeOfOmniVertex );
	OmniLight::OmniLightVertex * pOmniVertex = new OmniLight::OmniLightVertex[ iNbVertex ];

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
		pOmniVertex[i*4+1] = oBottomRight;
		pOmniVertex[i*4+2] = oBottomLeft;
		pOmniVertex[i*4+3] = oTopLeft;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, iVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, iSizeOmniVertex, pOmniVertex, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(m_iOmniLightShaderColorAndInverseRadiusHandle);
	glEnableVertexAttribArray(m_iOmniLightShaderViewSpacePosAndMultiplierHandle);

	glVertexPointer(2, GL_FLOAT, iSizeOfOmniVertex, 0);

	glVertexAttribPointer(m_iOmniLightShaderColorAndInverseRadiusHandle, 4, GL_FLOAT, GL_FALSE, iSizeOfOmniVertex, (void*)( 2 * sizeof(float) ) );	
	glVertexAttribPointer(m_iOmniLightShaderViewSpacePosAndMultiplierHandle, 4, GL_FLOAT, GL_FALSE, iSizeOfOmniVertex, (void*)( 6 * sizeof(float) ) );	

	glDrawArrays(GL_QUADS, 0, iNbVertex );

	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableVertexAttribArray(m_iOmniLightShaderColorAndInverseRadiusHandle);
	glDisableVertexAttribArray(m_iOmniLightShaderViewSpacePosAndMultiplierHandle);

	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	glDeleteBuffers(1, &iVertexBufferId);

	delete [] pOmniVertex;

	glEnable(GL_CULL_FACE);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------	
void DeferredRenderer::RenderSpotLights( std::vector< SpotLight::SpotLightQuad > vSpotLightQuads, unsigned int iColorAndInverseRadiusHandle, unsigned int iViewSpacePosAndMultiplierHandle, unsigned int iViewSpaceDirHandle, unsigned int iCosInAndOutHandle )
{
	Engine const& rEngine = Engine::GetInstance();
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,0,iWindowHeight,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	glDisable(GL_CULL_FACE);

	float fHalfWidth = iWindowWidth * 0.5f;
	float fHalfHeight = iWindowHeight * 0.5f; 

	GLuint iVertexBufferId;	
	glGenBuffers(1,  &iVertexBufferId);

	unsigned int iNbVertex = vSpotLightQuads.size() * 4;
	unsigned int iSizeOfSpotVertex = sizeof(SpotLight::SpotLightVertex);

	unsigned int iSizeSpotVertex = ( iNbVertex * iSizeOfSpotVertex );
	SpotLight::SpotLightVertex * pOmniVertex = new SpotLight::SpotLightVertex[ iNbVertex ];

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
		pOmniVertex[i*4+1] = oBottomRight;
		pOmniVertex[i*4+2] = oBottomLeft;
		pOmniVertex[i*4+3] = oTopLeft;
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

	glDrawArrays(GL_QUADS, 0, iNbVertex );

	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableVertexAttribArray( iColorAndInverseRadiusHandle );
	glDisableVertexAttribArray( iViewSpacePosAndMultiplierHandle );
	glDisableVertexAttribArray( iViewSpaceDirHandle );
	glDisableVertexAttribArray( iCosInAndOutHandle );
	
	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	glDeleteBuffers(1, &iVertexBufferId);

	delete [] pOmniVertex;

	glEnable(GL_CULL_FACE);
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

	glDisable(GL_DEPTH_TEST);

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
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void DeferredRenderer::DrawFrustum( const vec3 * pPoints, const vec3& f3Pos )
{
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
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::DisplayText( const std::string& sText, int iPosX, int iPosY )
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

	m_oFont->Begin();
	m_oFont->TextOut( sText, iPosX, iPosY, 0);

	Texture2D::Desactivate();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::PrepareOmniLights( const std::vector< OmniLight* >& oOmniLights, const AbstractCamera & rCamera, const Frustum& oViewFrustum, const float4x4& mModelView, const float4x4& mModelViewProjection )
{
	m_vOmniLightQuads.clear();
	std::vector< OmniLight* >::const_iterator oLightIt = oOmniLights.begin();
	std::vector< OmniLight* >::const_iterator oLightItEnd = oOmniLights.end();

	while( oLightIt != oLightItEnd )
	{
		OmniLight * pLight = (*oLightIt);
		
		vec3 f3Pos = pLight->GetPos();
		float fRadius = pLight->GetRadius(); 
		
		if(	oViewFrustum.sphereInFrustum( vec3(f3Pos.x, f3Pos.y, f3Pos.z), fRadius ) )
		{
			vec3 vLightToView = rCamera.GetPos() - f3Pos;
			float fLength = length(vLightToView);

			OmniLight::OmniLightQuad oQuad;

			//computing view space position
			vec4 vViewSpacePos = mModelView * vec4( f3Pos.x, f3Pos.y, f3Pos.z, 1.0 );
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

				vec4 oScreenPos = mModelViewProjection * vec4(oShiftedPos.x, oShiftedPos.y, oShiftedPos.z, 1.0 );
				oScreenPos = oScreenPos / oScreenPos.w;

				vec3 vLightRight = oShiftedPos + fRadius * rCamera.GetRight();
				vec4 oScreenRightPos = mModelViewProjection * vec4(vLightRight.x, vLightRight.y, vLightRight.z, 1.0 );
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
void DeferredRenderer::PrepareAndRenderSpotShadows( const std::vector< SpotShadow* >& oSpotShadows, const AbstractCamera & rCamera, const Frustum& oViewFrustum, const float4x4& mModelView, const float4x4& mModelViewProjection )
{
	std::vector< SpotShadow* >::const_iterator oLightIt = oSpotShadows.begin();
	std::vector< SpotShadow* >::const_iterator oLightItEnd = oSpotShadows.end();

	while( oLightIt != oLightItEnd )
	{
		SpotLight * pLight = (*oLightIt);
		SpotLight::SpotLightQuad oQuad;
		if( ComputeOneSpotBoundingQuad( pLight, rCamera, oViewFrustum, mModelView, mModelViewProjection, oQuad ) )
		{
			std::vector< SpotLight::SpotLightQuad > oQuadVector;
			oQuadVector.push_back( oQuad );

			(*oLightIt)->ActivateDepthTexture();
			mat4 mShadowMatrix = !transpose(mModelViewProjection) * (*oLightIt)->GetMatrix();
			m_pSpotShadowShader->setUniformMatrix4fv( m_iSpotShadowShaderShadowMatrixHandle, mShadowMatrix );


			RenderSpotLights( oQuadVector, m_iSpotShadowShaderColorAndInverseRadiusHandle, m_iSpotShadowShaderViewSpacePosAndMultiplierHandle, m_iSpotShadowShaderViewSpaceDirHandle, m_iSpotShadowShaderCosInAndOutHandle );
		}
		++oLightIt;
	}
}
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::PrepareSpotLights( const std::vector< SpotLight* >& oSpotLights, const AbstractCamera & rCamera, const Frustum& oViewFrustum, const float4x4& mModelView, const float4x4& mModelViewProjection )
{
	m_vSpotLightQuads.clear();
	std::vector< SpotLight* >::const_iterator oLightIt = oSpotLights.begin();
	std::vector< SpotLight* >::const_iterator oLightItEnd = oSpotLights.end();

	while( oLightIt != oLightItEnd )
	{
		SpotLight * pLight = (*oLightIt);
		SpotLight::SpotLightQuad oQuad;
		if( ComputeOneSpotBoundingQuad( pLight, rCamera, oViewFrustum, mModelView, mModelViewProjection, oQuad ) )
		{
			m_vSpotLightQuads.push_back( oQuad );
		}
		++oLightIt;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool DeferredRenderer::ComputeOneSpotBoundingQuad( SpotLight* pLight, AbstractCamera const& rCamera, Frustum const& oViewFrustum, float4x4 const& mModelView, float4x4 const& mModelViewProjection, SpotLight::SpotLightQuad& oQuad )
{
	vec3 f3Pos = pLight->GetPos();
	float fRadius = pLight->GetRadius(); 
		
	const float * pBoundingBox = pLight->GetBoundingBox();

	if(	oViewFrustum.cubeInFrustum( pBoundingBox[0], pBoundingBox[1],pBoundingBox[2],pBoundingBox[3],pBoundingBox[4],pBoundingBox[5] ) )
	{
		vec3 vCameraPos = rCamera.GetPos();
		vec3 vLightToView = vCameraPos - f3Pos;

		float fLength = length(vLightToView);

		//computing view space position
		vec4 vViewSpacePos = mModelView * vec4( f3Pos.x, f3Pos.y, f3Pos.z, 1.0f );
		vec3 vRotation = pLight->GetRotation();

		vec4 vViewSpaceDir = mModelView * rotateY( vRotation.y * (float)M_PI / 180.0f ) * rotateX( vRotation.x * (float)M_PI / 180.0f ) * vec4( 0.0f, 0.0f, -1.0f, 0.0f );

		oQuad.vViewSpaceLightPos = vec3( vViewSpacePos.x, vViewSpacePos.y, vViewSpacePos.z );
		oQuad.vViewSpaceLightDir = vec3( vViewSpaceDir.x, vViewSpaceDir.y, vViewSpaceDir.z );
		oQuad.vColor = pLight->GetColor();
		oQuad.fInverseRadius = 1.0f / fRadius;
		oQuad.fMultiplier = pLight->GetMultiplier();
		oQuad.vCosInAndOut = vec2( pLight->GetCosInnerAngle(), pLight->GetCosOuterAngle() );

		if( vCameraPos.x > pBoundingBox[0] && vCameraPos.x < pBoundingBox[1] && vCameraPos.y > pBoundingBox[2] && vCameraPos.y < pBoundingBox[3] && vCameraPos.z > pBoundingBox[4] && vCameraPos.z < pBoundingBox[5] )
		{
			oQuad.vLeftRightTopBottom = vec4( -1.0f, 1.0f, 1.0f, -1.0f );
		}
		else
		{
			const vec3 * pPoints = pLight->GetFarPlanePoints();

			float fLeft, fRight, fBottom, fTop;
			vec4 oScreenPos = mModelViewProjection * vec4(f3Pos.x, f3Pos.y, f3Pos.z, 1.0f );
			oScreenPos = oScreenPos / abs(oScreenPos.w);
			fLeft = fRight = oScreenPos.x;
			fBottom = fTop = oScreenPos.y;

			for( unsigned int i = 0; i < 4; ++i )
			{
				oScreenPos = mModelViewProjection * vec4(pPoints[i].x, pPoints[i].y, pPoints[i].z, 1.0f );
				oScreenPos = oScreenPos / abs(oScreenPos.w);
					
				fLeft = min(fLeft, oScreenPos.x);
				fRight = max(fRight, oScreenPos.x);

				fBottom = min(fBottom, oScreenPos.y);
				fTop = max(fTop, oScreenPos.y);
			}
			oQuad.vLeftRightTopBottom = vec4( fLeft, fRight, fTop, fBottom );
		}
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::RenderShadowMaps( const std::vector< SceneMesh* >& oSceneMeshes, SceneGraph& rSceneGraph, OpenGLContext& rRenderingContext )
{
	glCullFace( GL_FRONT );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

	std::vector< SpotShadow* > oSpotShadows = rSceneGraph.GetSpotShadows();
	std::vector< SpotShadow* >::iterator oSpotIt = oSpotShadows.begin();
	while( oSpotIt != oSpotShadows.end() )
	{
		SpotShadow * oSPot = *oSpotIt;
		
		vec3 oLightPos = oSPot->GetPos();
		vec3 oLightRotation = oSPot->GetRotation();
		rRenderingContext.Reshape( 512, 512, 2.0f * acosf( oSPot->GetCosOuterAngle()  ) / (float)M_PI * 180.0f, 0.15, oSPot->GetRadius() );
		glRotatef( -oLightRotation.x, 1.0,0.0,0.0 );
		glRotatef( oLightRotation.y, 0.0,1.0,0.0 );

		glTranslatef( -oLightPos.x, -oLightPos.y, -oLightPos.z );

		float4x4 mLightProjection;
		glGetFloatv ( GL_PROJECTION_MATRIX, mLightProjection );
		float4x4 mLightModelView;
		glGetFloatv ( GL_MODELVIEW_MATRIX, mLightModelView );
		oSPot->SetMatrix( transpose(transpose(mLightProjection) * transpose( mLightModelView ) ) );
	
		oSPot->ActivateBuffer();
		glClear( GL_DEPTH_BUFFER_BIT );	
		
		std::vector< SceneMesh* >::const_iterator oShadowMeshIt = oSceneMeshes.begin();
		while( oShadowMeshIt != oSceneMeshes.end() )
		{
			(*oShadowMeshIt)->Draw( EffectTechnique::E_RENDER_SHADOW_MAP );
			++oShadowMeshIt;
		}

		oSPot->DesactivateBuffer();
		++oSpotIt;
	}

	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	glCullFace( GL_BACK );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DeferredRenderer::Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );	
	
	SceneGraph & rSceneGraph = Engine::GrabInstance().GrabSceneGraph();
	const std::vector< SceneMesh* >& oSceneMeshes = rSceneGraph.GetSceneMeshes();
	
	Engine const& rEngine = Engine::GetInstance();
	
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	AbstractCamera & rCamera = rEngine.GetCurrentCamera();
	OpenGLContext& rRenderingContext = rEngine.GrabRenderingContext(); 

	RenderShadowMaps(oSceneMeshes,rSceneGraph,rRenderingContext);

	rRenderingContext.Reshape( iWindowWidth, iWindowHeight, rCamera.GetFOV(), rCamera.GetNear(), rCamera.GetFar() );	
	rCamera.LookAt();

	// Retrieving scene matrices
	float4x4 mProjection, mInvProjection;
	glGetFloatv ( GL_PROJECTION_MATRIX, mProjection );
	mInvProjection = !mProjection;

	float4x4 mModelView;
	glGetFloatv ( GL_MODELVIEW_MATRIX, mModelView );
	mModelView = transpose( mModelView );

	float4x4 mModelViewProjection = transpose(mProjection) * mModelView;

	Frustum oViewFrustum;
	oViewFrustum.loadFrustum(transpose(mModelViewProjection));

	//GBuffer pass
	m_oGBuffer->Activate();
	
	//multiple render target, not useful for now. Do not delete, I might need the code... :)
	//GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
	//glDrawBuffers(2, buffers);

	//Render view-space normal and depth in 2 buffers
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	std::vector< SceneMesh* >::const_iterator oMeshIt = oSceneMeshes.begin();
	while( oMeshIt != oSceneMeshes.end() )
	{
		(*oMeshIt)->Draw( EffectTechnique::E_RENDER_GBUFFER );
		++oMeshIt;
	}

	m_oGBuffer->Desactivate();

	//Lighting pass

	//creates one quad per omni light
	PrepareOmniLights( rSceneGraph.GetOmniLights(), rCamera, oViewFrustum, mModelView, mModelViewProjection );

	//creates one quad per spot light
	PrepareSpotLights( rSceneGraph.GetSpotLights(), rCamera, oViewFrustum, mModelView, mModelViewProjection );
	//enable blending in order to add all the light contributions
	
	glEnable(GL_BLEND);
	glBlendFunc( GL_ONE, GL_ONE );
	glDisable( GL_DEPTH_TEST );

	glActiveTexture( GL_TEXTURE0 );
	m_oGBuffer->ActivateTexture(0);
	glActiveTexture( GL_TEXTURE1 );
	m_oGBuffer->ActivateDepthTexture();
	glActiveTexture( GL_TEXTURE0 );

	m_oLightBuffer->Activate();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	

	//Render Omni Lights
	if( !m_vOmniLightQuads.empty() )
	{
		m_pOmniLightShader->Activate();	
		m_pOmniLightShader->CommitStdUniforms();
		m_pOmniLightShader->setUniformMatrix4fv( m_iOmniLightShaderInvProjHandle, mInvProjection );
		m_pOmniLightShader->setUniformi( "iDebug", m_iDebugFlag );

		RenderOmniLights( m_vOmniLightQuads );
		
		m_pOmniLightShader->Desactivate();
	}
	//Render Spot Lights
	if( !m_vSpotLightQuads.empty() )
	{
		m_pSpotLightShader->Activate();	
		m_pSpotLightShader->CommitStdUniforms();
		m_pSpotLightShader->setUniformMatrix4fv( m_iSpotLightShaderInvProjHandle, mInvProjection );
		m_pSpotLightShader->setUniformi( "iDebug", m_iDebugFlag );
		RenderSpotLights( m_vSpotLightQuads, m_iSpotLightShaderColorAndInverseRadiusHandle, m_iSpotLightShaderViewSpacePosAndMultiplierHandle, m_iSpotLightShaderViewSpaceDirHandle, m_iSpotLightShaderCosInAndOutHandle );
		m_pSpotLightShader->Desactivate();
	}



	m_pSpotShadowShader->Activate();	
	m_pSpotShadowShader->CommitStdUniforms();
	m_pSpotShadowShader->setUniformMatrix4fv( m_iSpotShadowShaderInvProjHandle, mInvProjection );

	glActiveTexture( GL_TEXTURE2 );
	
	m_pSpotShadowShader->setUniformi( "iDebug", m_iDebugFlag );
	PrepareAndRenderSpotShadows( rSceneGraph.GetSpotShadows(), rCamera, oViewFrustum, mModelView, mModelViewProjection );
	m_pSpotShadowShader->Activate();	

	Texture2D::Desactivate();

	m_oLightBuffer->Desactivate();

	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Desactivate();
	glActiveTexture( GL_TEXTURE1 );
	Texture2D::Desactivate();

	glDisable(GL_BLEND);
	glEnable( GL_DEPTH_TEST );

	//Material pass
	//The material pass needs to fetch the light buffer
	glActiveTexture( GL_TEXTURE6 );
	m_oLightBuffer->ActivateTexture();
	glActiveTexture( GL_TEXTURE0 );	

	//Restoring perspective view
	rRenderingContext.Reshape( iWindowWidth, iWindowHeight, rCamera.GetFOV(), rCamera.GetNear(), rCamera.GetFar() );
	rCamera.LookAt();
	oMeshIt = oSceneMeshes.begin();	
	while( oMeshIt != oSceneMeshes.end() )
	{
		(*oMeshIt)->Draw( EffectTechnique::E_RENDER_OPAQUE );
		++oMeshIt;
	}

	glActiveTexture( GL_TEXTURE6 );
	Texture2D::Desactivate();
	glActiveTexture( GL_TEXTURE0 );

	//Render lights bounding quads
	if( m_iDebugFlag == 1 )
	{
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f,0.0f,1.0f,0.5f);

		std::vector< SpotLight* >::const_iterator oSpotIt = rSceneGraph.GetSpotLights().begin();
		while( oSpotIt != rSceneGraph.GetSpotLights().end() )
		{
			DrawFrustum( (*oSpotIt)->GetFarPlanePoints(), (*oSpotIt)->GetPos() );
			++oSpotIt;
		}

		std::vector< SpotShadow* >::const_iterator oSpotShadowIt = rSceneGraph.GetSpotShadows().begin();
		while( oSpotShadowIt != rSceneGraph.GetSpotShadows().end() )
		{
			DrawFrustum( (*oSpotShadowIt)->GetFarPlanePoints(), (*oSpotShadowIt)->GetPos() );
			++oSpotShadowIt;
		}

		glColor4f(0.0f,1.0f,0.0f,0.5f);
		std::vector< OmniLight::OmniLightQuad >::iterator oOmniIt =  m_vOmniLightQuads.begin();
		while( oOmniIt != m_vOmniLightQuads.end() )
		{
			vec3 vData = vec3( (*oOmniIt).vScreenSpaceQuadCenter.x,(*oOmniIt).vScreenSpaceQuadCenter.y, (*oOmniIt).fHalfWidth );
			DrawScreenSpaceQuad( iWindowWidth, iWindowHeight, vData );
			++oOmniIt;
		}
	}

	m_fFrameCount++;
	if( m_fFrameCount >= m_fMaxFrame )
	{
		m_fFrameTime = m_oTimer->Stop() / m_fMaxFrame;
		m_fFrameCount = 0.0;
		m_oTimer->Start();
	}

	//Profiling infos
	std::stringstream oStream;
	oStream << "GPU:" << m_fFrameTime << "ms";
	DisplayText( oStream.str(), iWindowWidth - 200, 50);

	std::stringstream oStream2;
	oStream2 << "Displaying " << m_vOmniLightQuads.size() << " of " << rSceneGraph.GetOmniLights().size() << " omni lights.";
	DisplayText( oStream2.str(), iWindowWidth - 200, 70);
}