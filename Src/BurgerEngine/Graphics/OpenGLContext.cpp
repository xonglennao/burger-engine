#include "OpenGLContext.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Input/EventManager.h"



//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
OpenGLContext::OpenGLContext()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool OpenGLContext::Initialize( const unsigned int a_iWindowWidth, const unsigned int a_iWindowHeight )
{
	///Initialisation of Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error : " << glewGetErrorString(err) << std::endl;
		return false;
	}

	//Set OpenGL parameter
	/// \todo More flexible parameter??
	glClearColor(0.f,0.f,0.f,0.f);
	glClearDepth(1.f);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	
	glDisable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	//Reshape Once for start
	ReshapeGl(a_iWindowWidth,a_iWindowHeight);

	//Register Reshape method
	EventManager& rEventManager = Engine::GrabInstance().GrabEventManager();
	rEventManager.RegisterCallbackResize(EventManager::CallbackResize(this,&OpenGLContext::ReshapeGl));

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void OpenGLContext::Terminate()
{
	//Register Reshape method
	EventManager& rEventManager = Engine::GrabInstance().GrabEventManager();
	rEventManager.UnRegisterCallbackResize(EventManager::CallbackResize(this,&OpenGLContext::ReshapeGl));
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool OpenGLContext::ReshapeGl(unsigned int a_uWidth, unsigned int a_uHeight)
{
	float fRatio = static_cast<float>(a_uWidth)/static_cast<float>(a_uHeight);
	glViewport(0, 0, static_cast<GLint>(a_uWidth), static_cast<GLint>(a_uHeight));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/// \todo hard coded value, but it will be camera class value
	gluPerspective(45.0f, fRatio, 0.1f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool OpenGLContext::CheckError()
{
	GLenum err = glGetError();
	if(err!=GL_NO_ERROR)
	{
		std::cerr << "[ERROR][GL] :" << std::endl;
		std::cerr << gluErrorString(err) << std::endl;
		return true;
	}

	return false;
}