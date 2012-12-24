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
	/*
	///Initialisation of Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error : " << glewGetErrorString(err) << std::endl;
		return false;
	}
	*/
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
	//are these line usefull??
	//ReshapeGl(a_iWindowWidth,a_iWindowHeight);

	//Register Reshape method
	//EventManager& rEventManager = Engine::GrabInstance().GrabEventManager();
	//rEventManager.RegisterCallbackResize(EventManager::CallbackResize(this,&OpenGLContext::ReshapeGl));

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void OpenGLContext::Terminate()
{
	//Register Reshape method
	EventManager& rEventManager = Engine::GrabInstance().GrabEventManager();
	//rEventManager.UnRegisterCallbackResize(EventManager::CallbackResize(this,&OpenGLContext::ReshapeGl));
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