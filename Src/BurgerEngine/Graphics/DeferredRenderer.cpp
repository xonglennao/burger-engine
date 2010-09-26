#include "BurgerEngine/Graphics/DeferredRenderer.h"

#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/SceneGraph.h"

#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Graphics/OpenGLContext.h"
#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/Graphics/FBO.h"
#include "BurgerEngine/Graphics/ShaderManager.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/Texture2D.h"

#include "BurgerEngine/Core/Timer.h"

#include "BurgerEngine/External/Math/Vector.h"
#include "BurgerEngine/External/GLFont/glfont.h"

#include <sstream>


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
	m_oGBuffer->GenerateGBuffer();

	m_oLightBuffer = new FBO( iWindowWidth, iWindowHeight, FBO::E_FBO_2D );
	m_oLightBuffer->GenerateColorOnly();

	//loading engine shaders

	m_pOmniLightShader = ShaderManager::GrabInstance().addShader( "OmniLightShader", "../Data/Shaders/OmniLight.vert", "../Data/Shaders/OmniLight.frag" );
	m_pOmniLightShader->Activate();
	m_pOmniLightShader->setUniformTexture("sNormalSampler",0);
	m_pOmniLightShader->setUniformTexture("sDepthSampler",1);
	m_pOmniLightShader->Desactivate();
	
	//loading font
	//Create font
	GLuint iFontId;
	glGenTextures(1, &iFontId);
	m_oFont = new PixelPerfectGLFont();
	m_oFont->Create("../Data/Fonts/lucida_sans.glf", iFontId);

	m_oTimer = new Timer();

	m_pLightZeros = new GLfloat[4];
	m_pLightZeros[0] = 0.0;
	m_pLightZeros[1] = 0.0;
	m_pLightZeros[2] = 0.0;
	m_pLightZeros[3] = 1.0;

	m_pLightDir	= new GLfloat[3];
	m_pLightDir[0] = 0.0;
	m_pLightDir[1] = 0.0;
	m_pLightDir[2] = -1.0;
}

DeferredRenderer::~DeferredRenderer()
{
	delete m_oGBuffer;
	m_oGBuffer = NULL;
	
	delete [] m_pLightZeros;
	m_pLightZeros = NULL;

	delete [] m_pLightDir;
	m_pLightDir = NULL;

	delete m_oFont;
	m_oFont = NULL;

	delete m_oTimer;
	m_oTimer = NULL;
}

void DeferredRenderer::DrawScreenSpaceQuad( int iWindowWidth, int iWindowHeight )
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
	m_oFont->TextOut( sText, iPosX, iPosY, 0.0);

	Texture2D::Desactivate();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void DeferredRenderer::Render()
{
	m_oTimer->Start();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );	
	
	SceneGraph & rSceneGraph = Engine::GrabInstance().GrabSceneGraph();
	const std::vector< SceneMesh* >& oSceneMeshes = rSceneGraph.GetSceneMeshes();
	const std::vector< SceneLight* >& oSceneLights = rSceneGraph.GetSceneLights();
	
	Engine const& rEngine = Engine::GetInstance();
	
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	AbstractCamera & rCamera = rEngine.GetCurrentCamera();
	OpenGLContext& rRenderingContext = rEngine.GrabRenderingContext(); 

	rRenderingContext.ReshapeGl( iWindowWidth, iWindowHeight );
	
	rCamera.LookAt();
	
	// Compute scene matrices
	GLfloat mGLProjection[16];
	glGetFloatv ( GL_PROJECTION_MATRIX, mGLProjection ); // grab projection matrix

	float4x4 mProjection = float4x4(
							mGLProjection[0], mGLProjection[1], mGLProjection[2],mGLProjection[3],
							mGLProjection[4], mGLProjection[5], mGLProjection[6],mGLProjection[7],
							mGLProjection[8], mGLProjection[9], mGLProjection[10],mGLProjection[11],
							mGLProjection[12], mGLProjection[13], mGLProjection[14],mGLProjection[15] );

	GLfloat* mGLInvProj = (float*)!mProjection;

	unsigned int i = 0;
	std::vector< SceneLight* >::const_iterator oLightIt = oSceneLights.begin();
	while( oLightIt != oSceneLights.end() )
	{
		SceneLight * pLight = (*oLightIt);
		vec4 oPos = pLight->GetPos();
		vec3 oRotation = pLight->GetRotation();
		glPushMatrix();
			glTranslatef( oPos.x, oPos.y , oPos.z );
			glRotatef( oRotation.x, 1, 0, 0 );
			glRotatef( oRotation.y, 0, 1, 0 );
			glRotatef( oRotation.z, 0, 0, 1 );
			if( oPos.w == 1.0 )
			{
				glLightfv( GL_LIGHT0+i, GL_POSITION, m_pLightZeros );
			}
			else
			{
				GLfloat fPos[] = { 1.0, 0.0 , 0.0, 0.0 };
				glLightfv( GL_LIGHT0+i, GL_POSITION, fPos );
			}

			glLightfv( GL_LIGHT0+i, GL_SPOT_DIRECTION, m_pLightDir );
		glPopMatrix();
		++i;
		++oLightIt;
	}

	//GBuffer pass
	m_oGBuffer->Activate();
	
	//multiple render target, not useful for now. Do not delete, I might need the code... :)
	//GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
	//glDrawBuffers(2, buffers);

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
	glActiveTexture( GL_TEXTURE0 );
	m_oGBuffer->ActivateTexture(0);
	glActiveTexture( GL_TEXTURE1 );
	m_oGBuffer->ActivateDepthTexture();
	glActiveTexture( GL_TEXTURE0 );

	m_pOmniLightShader->Activate();

	m_pOmniLightShader->setUniformf( "fWindowWidth", iWindowWidth );
	m_pOmniLightShader->setUniformf( "fWindowHeight", iWindowHeight );
	m_pOmniLightShader->setUniformMatrix4fv( "mInvProj", mGLInvProj );
	m_pOmniLightShader->setUniformi( "iDebug", m_iDebugFlag );

	m_oLightBuffer->Activate();		
	DrawScreenSpaceQuad( iWindowWidth, iWindowHeight );
	m_oLightBuffer->Desactivate();

	m_pOmniLightShader->Desactivate();

	glActiveTexture( GL_TEXTURE0 );
	Texture2D::Desactivate();
	glActiveTexture( GL_TEXTURE1 );
	Texture2D::Desactivate();
	glActiveTexture( GL_TEXTURE0 );
	//m_oLightBuffer->ActivateTexture();
	//DrawScreenSpaceQuad( iWindowWidth, iWindowHeight );


	glActiveTexture( GL_TEXTURE6 );
	m_oLightBuffer->ActivateTexture();
	glActiveTexture( GL_TEXTURE0 );	
	//Material pass
	rRenderingContext.ReshapeGl( iWindowWidth, iWindowHeight );
	rCamera.LookAt();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		//std::vector< SceneMesh* >::const_iterator oMeshIt = oSceneMeshes.begin();
		oMeshIt = oSceneMeshes.begin();	
		while( oMeshIt != oSceneMeshes.end() )
		{
			(*oMeshIt)->Draw( EffectTechnique::E_RENDER_OPAQUE );
			++oMeshIt;
		}

	glActiveTexture( GL_TEXTURE6 );
	Texture2D::Desactivate();
	glActiveTexture( GL_TEXTURE0 );

	std::stringstream oStream;
	oStream << "GPU:" << m_oTimer->Stop() << "ms";
	DisplayText( oStream.str(), iWindowWidth - 200, 50);
}