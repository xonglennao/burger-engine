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
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------

float g_fHackMovingLights = 0.0f;

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
	m_iOmniLightShaderInvMVPHandle = glGetUniformLocation( m_pOmniLightShader->getHandle(), "mInvProj" );
	
	m_iOmniLightShaderColorAndInverseRadiusHandle = glGetAttribLocation( m_pOmniLightShader->getHandle(),"vColorAndInverseRadius");
	m_iOmniLightShaderViewSpacePosAndMultiplierHandle = glGetAttribLocation( m_pOmniLightShader->getHandle(),"vViewSpacePosAndMultiplier");

	m_pOmniLightShader->setUniformTexture("sNormalSampler",0);
	m_pOmniLightShader->setUniformTexture("sDepthSampler",1);

	m_pOmniLightShader->Desactivate();
	
	//loading font
	GLuint iFontId;
	glGenTextures(1, &iFontId);
	m_oFont = new PixelPerfectGLFont();
	m_oFont->Create("../Data/Fonts/lucida_sans.glf", iFontId);

	m_oTimer = new Timer();
	m_fMaxFrame = 15.0;
	m_fFrameCount = m_fMaxFrame;


}

DeferredRenderer::~DeferredRenderer()
{
	delete m_oGBuffer;
	m_oGBuffer = NULL;

	delete m_oFont;
	m_oFont = NULL;

	delete m_oTimer;
	m_oTimer = NULL;
}

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

void DeferredRenderer::RenderOmniLights( std::vector< SceneLight::OmniLightQuad > vOmniLightQuads )
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

	//vertex buffer size 
	unsigned int iSizeVertex = ( iNbVertex * 2 * sizeof(float) );
	float * pVertexPositions = new float[ iNbVertex * 2 ];

	unsigned int iSizeColorAndInvRadius = ( iNbVertex * 4 * sizeof(float) );
	float * pColorsAndInvRadius = new float[ iNbVertex * 4 ];

	unsigned int iSizeViewSpacePosAndMultiplier = ( iNbVertex * 4 * sizeof(float) );
	float * pViewSpacePosAndMultiplier = new float[ iNbVertex * 4 ];

	for(unsigned int i = 0; i < vOmniLightQuads.size(); ++i )
	{
		float x = fHalfWidth * (vOmniLightQuads[i].vScreenSpaceCenter.x + 1.0f);
		float y = fHalfHeight * (vOmniLightQuads[i].vScreenSpaceCenter.y + 1.0f);

		float fHalfSquare = fHalfWidth * vOmniLightQuads[i].fHalfWidth;

		float fRight = clamp(x+fHalfSquare,0.0f, (float)iWindowWidth);
		float fLeft = clamp(x-fHalfSquare,0.0f, (float)iWindowWidth);
		float fTop = clamp(y+fHalfSquare,0.0f, (float)iWindowHeight);
		float fBottom = clamp(y-fHalfSquare,0.0f, (float)iWindowHeight);

		pVertexPositions[i*8] = fRight;		
		pVertexPositions[i*8+1] = fTop;
		pVertexPositions[i*8+2] = fRight;
		pVertexPositions[i*8+3] = fBottom;
		pVertexPositions[i*8+4] = fLeft;
		pVertexPositions[i*8+5] = fBottom;
		pVertexPositions[i*8+6] = fLeft;
		pVertexPositions[i*8+7] = fTop;

		pColorsAndInvRadius[i*16] = vOmniLightQuads[i].vColor.x;
		pColorsAndInvRadius[i*16+1] = vOmniLightQuads[i].vColor.y;
		pColorsAndInvRadius[i*16+2] = vOmniLightQuads[i].vColor.z;
		pColorsAndInvRadius[i*16+3] = vOmniLightQuads[i].fInverseRadius;

		pColorsAndInvRadius[i*16+4] = vOmniLightQuads[i].vColor.x;
		pColorsAndInvRadius[i*16+5] = vOmniLightQuads[i].vColor.y;
		pColorsAndInvRadius[i*16+6] = vOmniLightQuads[i].vColor.z;
		pColorsAndInvRadius[i*16+7] = vOmniLightQuads[i].fInverseRadius;

		pColorsAndInvRadius[i*16+8] = vOmniLightQuads[i].vColor.x;
		pColorsAndInvRadius[i*16+9] = vOmniLightQuads[i].vColor.y;
		pColorsAndInvRadius[i*16+10] = vOmniLightQuads[i].vColor.z;
		pColorsAndInvRadius[i*16+11] = vOmniLightQuads[i].fInverseRadius;

		pColorsAndInvRadius[i*16+12] = vOmniLightQuads[i].vColor.x;
		pColorsAndInvRadius[i*16+13] = vOmniLightQuads[i].vColor.y;
		pColorsAndInvRadius[i*16+14] = vOmniLightQuads[i].vColor.z;
		pColorsAndInvRadius[i*16+15] = vOmniLightQuads[i].fInverseRadius;

		pViewSpacePosAndMultiplier[i*16] = vOmniLightQuads[i].vViewSpacePosition.x;
		pViewSpacePosAndMultiplier[i*16+1] = vOmniLightQuads[i].vViewSpacePosition.y;
		pViewSpacePosAndMultiplier[i*16+2] = vOmniLightQuads[i].vViewSpacePosition.z;
		pViewSpacePosAndMultiplier[i*16+3] = vOmniLightQuads[i].fMultiplier;

		pViewSpacePosAndMultiplier[i*16+4] = vOmniLightQuads[i].vViewSpacePosition.x;
		pViewSpacePosAndMultiplier[i*16+5] = vOmniLightQuads[i].vViewSpacePosition.y;
		pViewSpacePosAndMultiplier[i*16+6] = vOmniLightQuads[i].vViewSpacePosition.z;
		pViewSpacePosAndMultiplier[i*16+7] = vOmniLightQuads[i].fMultiplier;

		pViewSpacePosAndMultiplier[i*16+8] = vOmniLightQuads[i].vViewSpacePosition.x;
		pViewSpacePosAndMultiplier[i*16+9] = vOmniLightQuads[i].vViewSpacePosition.y;
		pViewSpacePosAndMultiplier[i*16+10] = vOmniLightQuads[i].vViewSpacePosition.z;
		pViewSpacePosAndMultiplier[i*16+11] = vOmniLightQuads[i].fMultiplier;

		pViewSpacePosAndMultiplier[i*16+12] = vOmniLightQuads[i].vViewSpacePosition.x;
		pViewSpacePosAndMultiplier[i*16+13] = vOmniLightQuads[i].vViewSpacePosition.y;
		pViewSpacePosAndMultiplier[i*16+14] = vOmniLightQuads[i].vViewSpacePosition.z;
		pViewSpacePosAndMultiplier[i*16+15] = vOmniLightQuads[i].fMultiplier;

	}
	
	glBindBuffer(GL_ARRAY_BUFFER, iVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, iSizeVertex+iSizeColorAndInvRadius+iSizeViewSpacePosAndMultiplier, pVertexPositions, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, iSizeVertex, iSizeColorAndInvRadius, pColorsAndInvRadius);
	glBufferSubData(GL_ARRAY_BUFFER, iSizeVertex+iSizeColorAndInvRadius, iSizeViewSpacePosAndMultiplier, pViewSpacePosAndMultiplier);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableVertexAttribArray(m_iOmniLightShaderColorAndInverseRadiusHandle);
	glEnableVertexAttribArray(m_iOmniLightShaderViewSpacePosAndMultiplierHandle);

	glVertexPointer(2, GL_FLOAT, 0, 0);
	
	glVertexAttribPointer(m_iOmniLightShaderColorAndInverseRadiusHandle,4,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)iSizeVertex);
	
	glVertexAttribPointer(m_iOmniLightShaderViewSpacePosAndMultiplierHandle,4,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)( iSizeVertex+iSizeColorAndInvRadius ) );	
	
	glDrawArrays(GL_QUADS, 0, iNbVertex );

	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableVertexAttribArray(m_iOmniLightShaderColorAndInverseRadiusHandle);
	glDisableVertexAttribArray(m_iOmniLightShaderViewSpacePosAndMultiplierHandle);

	glBindBuffer(GL_ARRAY_BUFFER, iNbVertex );

	glDeleteBuffers(1, &iVertexBufferId);

	delete [] pVertexPositions; 
	delete [] pColorsAndInvRadius;
	delete [] pViewSpacePosAndMultiplier;
	
	glEnable(GL_CULL_FACE);
}
	
void DeferredRenderer::DrawScreenSpaceQuad( int iWindowWidth, int iWindowHeight, vec3 vData )
{
	glPointSize(10);
	glDisable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,0,iWindowHeight,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float fHalfWidth = iWindowWidth * 0.5f;
	float fHalfHeight = iWindowHeight * 0.5f;

	glEnable(GL_BLEND);
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
	glColor4f(1.0f,0.0f,0.0f,0.5f);

	glVertex2f(fRight, fTop );
	glVertex2f(fRight,fBottom );
	glVertex2f(fLeft, fBottom );
	glVertex2f(fLeft, fTop );

	glEnd();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

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

void DeferredRenderer::PrepareOmniLights( const std::vector< SceneLight* >& oOmniLights, const AbstractCamera & rCamera, const Frustum& oViewFrustum, const float4x4& mModelView, const float4x4& mModelViewProjection )
{
	m_vOmniLightQuads.clear();
	std::vector< SceneLight* >::const_iterator oLightIt = oOmniLights.begin();
	std::vector< SceneLight* >::const_iterator oLightItEnd = oOmniLights.end();

	g_fHackMovingLights += 0.01f;

	while( oLightIt != oLightItEnd )
	{
		SceneLight * pLight = (*oLightIt);
		
		vec3 oPos = pLight->GetPos();
		oPos.x += 6.0 * sin( oPos.x + g_fHackMovingLights );
		oPos.z += 6.0 * cos( oPos.z + g_fHackMovingLights );
		float fRadius = pLight->GetRadius(); 
		
		if(	oViewFrustum.sphereInFrustum( vec3(oPos.x, oPos.y, oPos.z), fRadius ) )
		{
			vec3 vLightToView = rCamera.GetPos() - oPos;
			float fLength = length(vLightToView);

			SceneLight::OmniLightQuad oQuad;

			//computing view space position
			vec4 vViewSpacePos = transpose( mModelView ) * vec4( oPos.x, oPos.y, oPos.z, 1.0 );
			oQuad.vViewSpacePosition = vec3( vViewSpacePos.x, vViewSpacePos.y, vViewSpacePos.z );
			oQuad.vColor = pLight->GetColor();
			oQuad.fInverseRadius = 1.0f / fRadius;
			oQuad.fMultiplier = pLight->GeMultiplier();

			if( fLength <= fRadius )
			{
				oQuad.vScreenSpaceCenter = vec2( 0.0f, 0.0f );
				oQuad.fHalfWidth = 1.0f;
			}
			else
			{
				vLightToView = normalize( vLightToView );

				vec3 oShiftedPos = oPos + min( fLength, fRadius ) * vLightToView;

				vec4 oScreenPos = mModelViewProjection * vec4(oShiftedPos.x, oShiftedPos.y, oShiftedPos.z, 1.0 );
				oScreenPos = oScreenPos / oScreenPos.w;

				vec3 vLightRight = oShiftedPos + fRadius * rCamera.GetRight();
				vec4 oScreenRightPos = mModelViewProjection * vec4(vLightRight.x, vLightRight.y, vLightRight.z, 1.0 );
				oScreenRightPos = oScreenRightPos / oScreenRightPos.w;

				oQuad.vScreenSpaceCenter = vec2( oScreenPos.x, oScreenPos.y );
				oQuad.fHalfWidth = oScreenPos.x - oScreenRightPos.x;
			}
				m_vOmniLightQuads.push_back( oQuad );
		}
		++oLightIt;
	}
}

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

	rRenderingContext.ReshapeGl( iWindowWidth, iWindowHeight );
	
	rCamera.LookAt();

	// Retrieving scene matrices
	float4x4 mProjection;
	glGetFloatv ( GL_PROJECTION_MATRIX, mProjection );

	float4x4 mModelView;
	glGetFloatv ( GL_MODELVIEW_MATRIX, mModelView );

	float4x4 mModelViewProjection = transpose(mProjection) * transpose(mModelView);

	Frustum oViewFrustum;
	oViewFrustum.loadFrustum(transpose(mModelViewProjection));

	//GBuffer pass
	m_oGBuffer->Activate();
	
	//multiple render target, not useful for now. Do not delete, I might need the code... :)
	//GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
	//glDrawBuffers(2, buffers);

	//Render view-space normal and depth in 2 buffers
	glViewport( 0, 0, m_oGBuffer->GetWidth(), m_oGBuffer->GetHeight() );
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
	PrepareOmniLights( 	rSceneGraph.GetSceneLights(), rCamera, oViewFrustum, mModelView, mModelViewProjection );

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
	
	//Render Omni Lights

	m_pOmniLightShader->Activate();	
	m_pOmniLightShader->CommitStdUniforms();
	m_pOmniLightShader->setUniformMatrix4fv( m_iOmniLightShaderInvMVPHandle, !mProjection );
	m_pOmniLightShader->setUniformi( "iDebug", m_iDebugFlag );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	if( !m_vOmniLightQuads.empty() )
	{
		RenderOmniLights( m_vOmniLightQuads );
	}

	m_oLightBuffer->Desactivate();
	m_pOmniLightShader->Desactivate();

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
	rRenderingContext.ReshapeGl( iWindowWidth, iWindowHeight );
	rCamera.LookAt();
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	oMeshIt = oSceneMeshes.begin();	
	while( oMeshIt != oSceneMeshes.end() )
	{
		(*oMeshIt)->Draw( EffectTechnique::E_RENDER_OPAQUE );
		++oMeshIt;
	}

	glActiveTexture( GL_TEXTURE6 );
	Texture2D::Desactivate();
	glActiveTexture( GL_TEXTURE0 );

	//Render omni lights bounding quads
	if( m_iDebugFlag == 2 )
	{
		std::vector< SceneLight::OmniLightQuad >::iterator oOmniIt =  m_vOmniLightQuads.begin();
		while( oOmniIt != m_vOmniLightQuads.end() )
		{
			vec3 vData = vec3( (*oOmniIt).vScreenSpaceCenter.x,(*oOmniIt).vScreenSpaceCenter.y, (*oOmniIt).fHalfWidth );
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
	oStream2 << "Displaying " << m_vOmniLightQuads.size() << " of " << rSceneGraph.GetSceneLights().size() << " lights.";
	DisplayText( oStream2.str(), iWindowWidth - 200, 70);
}