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

void DeferredRenderer::DrawScreenSpaceQuad( int iWindowWidth, int iWindowHeight, vec3 vData )
{
	glPointSize(5);
		glDisable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	  
	glViewport(0,0,iWindowWidth,iWindowHeight);
	glOrtho(0,iWindowWidth,0,iWindowHeight,-0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float fHalfWidth = iWindowWidth * 0.5f;
	float fHalfHeight = iWindowHeight * 0.5f;

	float x,y;

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable(GL_DEPTH_TEST);


	glBegin(GL_POINTS);
	x = fHalfWidth * (vData.x + 1);
	y = fHalfHeight * (vData.y + 1);	
	glVertex2f(x, y );	
	glEnd();

	glBegin(GL_QUADS);

	float fHalfSquare = fHalfWidth * vData.z;

	glVertex2f(x+fHalfSquare, y+fHalfSquare );
	
	x = fHalfWidth * (vData.x + 1);
	y = fHalfHeight * (vData.y + 1);		
	glVertex2f(x+fHalfSquare, y-fHalfSquare );

	x = fHalfWidth * (vData.x + 1);
	y = fHalfHeight * (vData.y + 1);		
	glVertex2f(x-fHalfSquare, y-fHalfSquare );

	x = fHalfWidth * (vData.x + 1);
	y = fHalfHeight * (vData.y + 1);		
	glVertex2f(x-fHalfSquare, y+fHalfSquare );

	glEnd();
	


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

void DeferredRenderer::Render()
{
	//DEBUG LIGHT
	std::vector< vec3 > vLightPos;
	
	
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

	// Retrieving scene matrices
	float4x4 mProjection;
	glGetFloatv ( GL_PROJECTION_MATRIX, mProjection );

	float4x4 mModelView;
	glGetFloatv ( GL_MODELVIEW_MATRIX, mModelView );

	float4x4 mModelViewProjection = transpose(mProjection) * transpose(mModelView);

	vec4 oViewPos = vec4( rCamera.GetPos(), 1.0 );

	unsigned int i = 0;
	std::vector< SceneLight* >::const_iterator oLightIt = oSceneLights.begin();
	while( oLightIt != oSceneLights.end() )
	{
		SceneLight * pLight = (*oLightIt);
		vec4 oPos = pLight->GetPos();
		float fRadius = pLight->GetRadius(); 

		vec4 oScreenPos = mModelViewProjection * oPos ;
		oScreenPos = oScreenPos / oScreenPos.w;

		vec4 vLightRight = oPos + vec4( fRadius * rCamera.GetRight(), 0.0 );
		vec4 oScreenRightPos = mModelViewProjection * vLightRight ;
		oScreenRightPos = oScreenRightPos / oScreenRightPos.w;

		float fHalfSquareLength = oScreenPos.x - oScreenRightPos.x;

		vLightPos.push_back( vec3( oScreenPos.x, oScreenPos.y,fHalfSquareLength ) ); 

		glPushMatrix();
			glTranslatef( oPos.x, oPos.y , oPos.z );
			glLightfv( GL_LIGHT0+i, GL_POSITION, m_pLightZeros );
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

	m_pOmniLightShader->setUniformf( "fWindowWidth", (float)iWindowWidth );
	m_pOmniLightShader->setUniformf( "fWindowHeight", (float)iWindowHeight );
	m_pOmniLightShader->setUniformMatrix4fv( "mInvProj", !mProjection );
	m_pOmniLightShader->setUniformi( "iDebug", m_iDebugFlag );

	m_oLightBuffer->Activate();		
	DrawFullScreenQuad( iWindowWidth, iWindowHeight );
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

	std::vector<vec3>::iterator oLightPosIt = vLightPos.begin();

	bool bFlag = true;

	while( oLightPosIt != vLightPos.end() )
	{
		if(bFlag)
		{
			glColor3f(0.0f,1.0f,0.0f);
		}
		else
		{
			glColor3f(1.0f,0.0f,0.0f);
		}

		bFlag = !bFlag;

		//vec3 oData = (*oLightPosIt);
		DrawScreenSpaceQuad( iWindowWidth, iWindowHeight, (*oLightPosIt) );

		++oLightPosIt;
	}
	

}