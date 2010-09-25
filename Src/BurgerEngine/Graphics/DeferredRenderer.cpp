#include "BurgerEngine/Graphics/DeferredRenderer.h"

#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Core/AbstractCamera.h"
#include "BurgerEngine/Core/SceneGraph.h"

#include "BurgerEngine/Graphics/OpenGLContext.h"
#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/Graphics/FBO.h"
#include "BurgerEngine/Graphics/ShaderManager.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/Texture2D.h"

#include "BurgerEngine/External/Math/Vector.h"



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
	m_pOmniLightShader->setUniformTexture("sColorSampler",0);
	m_pOmniLightShader->setUniformTexture("sNormalSampler",1);
	m_pOmniLightShader->setUniformTexture("sDepthSampler",2);
	m_pOmniLightShader->Desactivate();
	
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

}

void DeferredRenderer::DrawScreenSpaceQuad(int iWindowWidth, int iWindowHeight )
{
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,0,iWindowHeight,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	  
	glDisable(GL_BLEND);
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
	glEnable(GL_BLEND);
}

void DeferredRenderer::Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );	
	
	SceneGraph & rSceneGraph = Engine::GrabInstance().GrabSceneGraph();
	const std::vector< SceneMesh* >& oSceneMeshes = rSceneGraph.GetSceneMeshes();
	const std::vector< SceneLight* >& oSceneLights = rSceneGraph.GetSceneLights();
	
	Engine const& rEngine = Engine::GetInstance();
	
	unsigned int iWindowWidth = rEngine.GetWindowWidth();
	unsigned int iWindowHeight = rEngine.GetWindowHeight();

	AbstractCamera & rCamera = rEngine.GetCurrentCamera();
	OpenGLContext& rRenderingContext = rEngine.GrabRenderingContext(); 

	rRenderingContext.ReshapeGl( m_oGBuffer->GetWidth(), m_oGBuffer->GetHeight() );
	
	rCamera.Update();
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

	//Fill GBuffer
	m_oGBuffer->Activate();
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT };
	glDrawBuffers(2, buffers);
		glViewport( 0, 0, m_oGBuffer->GetWidth(), m_oGBuffer->GetHeight() );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		std::vector< SceneMesh* >::const_iterator oMeshIt = oSceneMeshes.begin();
		while( oMeshIt != oSceneMeshes.end() )
		{
			(*oMeshIt)->Draw();
			++oMeshIt;
		}
	m_oGBuffer->Desactivate();

	if( m_iDebugFlag == 0 )
	{
		//Lighting
		glActiveTexture( GL_TEXTURE0 );
		m_oGBuffer->ActivateTexture(0);
		glActiveTexture( GL_TEXTURE1 );
		m_oGBuffer->ActivateTexture(1);
		glActiveTexture( GL_TEXTURE2 );
		m_oGBuffer->ActivateDepthTexture();
		glActiveTexture( GL_TEXTURE0 );

		m_pOmniLightShader->Activate();

		m_pOmniLightShader->setUniformf( "fWindowWidth", iWindowWidth );
		m_pOmniLightShader->setUniformf( "fWindowHeight", iWindowHeight );
		m_pOmniLightShader->setUniformMatrix4fv( "mInvProj", mGLInvProj );
		m_pOmniLightShader->setUniformi( "bDebug", 0 );
		DrawScreenSpaceQuad( rEngine.GetWindowWidth(), rEngine.GetWindowHeight() );
		m_pOmniLightShader->Desactivate();
	}
	if( m_iDebugFlag == 1 )
	{
		//Lighting
		glActiveTexture( GL_TEXTURE0 );
		m_oGBuffer->ActivateTexture(0);
		glActiveTexture( GL_TEXTURE1 );
		m_oGBuffer->ActivateTexture(1);
		glActiveTexture( GL_TEXTURE2 );
		m_oGBuffer->ActivateDepthTexture();
		glActiveTexture( GL_TEXTURE0 );

		m_pOmniLightShader->Activate();

		m_pOmniLightShader->setUniformf( "fWindowWidth", iWindowWidth );
		m_pOmniLightShader->setUniformf( "fWindowHeight", iWindowHeight );
		m_pOmniLightShader->setUniformMatrix4fv( "mInvProj", mGLInvProj );
		m_pOmniLightShader->setUniformi( "bDebug", 1 );
		DrawScreenSpaceQuad( rEngine.GetWindowWidth(), rEngine.GetWindowHeight() );
		m_pOmniLightShader->Desactivate();
	}
	if(m_iDebugFlag == 2 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_oGBuffer->ActivateTexture(0);
		glActiveTexture( GL_TEXTURE1 );
		Texture2D::Desactivate();
		glActiveTexture( GL_TEXTURE2 );
		Texture2D::Desactivate();
		glActiveTexture( GL_TEXTURE0 );
		DrawScreenSpaceQuad( rEngine.GetWindowWidth(), rEngine.GetWindowHeight() );
	}
	if(m_iDebugFlag == 3 )
	{
		glActiveTexture( GL_TEXTURE0 );
		m_oGBuffer->ActivateTexture(1);
		glActiveTexture( GL_TEXTURE1 );
		Texture2D::Desactivate();
		glActiveTexture( GL_TEXTURE2 );
		Texture2D::Desactivate();
		glActiveTexture( GL_TEXTURE0 );
		DrawScreenSpaceQuad( rEngine.GetWindowWidth(), rEngine.GetWindowHeight() );
	}
	if( m_iDebugFlag == 4 )
	{
		//Lighting
		glActiveTexture( GL_TEXTURE0 );
		m_oGBuffer->ActivateTexture(0);
		glActiveTexture( GL_TEXTURE1 );
		m_oGBuffer->ActivateTexture(1);
		glActiveTexture( GL_TEXTURE2 );
		m_oGBuffer->ActivateDepthTexture();
		glActiveTexture( GL_TEXTURE0 );

		m_pOmniLightShader->Activate();

		m_pOmniLightShader->setUniformf( "fWindowWidth", iWindowWidth );
		m_pOmniLightShader->setUniformf( "fWindowHeight", iWindowHeight );
		m_pOmniLightShader->setUniformMatrix4fv( "mInvProj", mGLInvProj );
		m_pOmniLightShader->setUniformi( "bDebug", 2 );
		DrawScreenSpaceQuad( rEngine.GetWindowWidth(), rEngine.GetWindowHeight() );
		m_pOmniLightShader->Desactivate();
	}
}