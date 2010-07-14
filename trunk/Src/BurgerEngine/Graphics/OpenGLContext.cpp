
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

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
bool OpenGLContext::Init()
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
	glClearColor(1.0f,1.0f,1.0f,1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	//Register Reshape method
	EventManager& rEventManager = Engine::GetInstance().GrabEventManager();
	rEventManager.RegisterCallbacResize(EventManager::CallbackResize(this,&OpenGLContext::ReshapeGl));

	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool OpenGLContext::ReshapeGl(unsigned int a_uHeight, unsigned int a_uWidth)
{
	float fRatio = static_cast<float>(a_uWidth)/static_cast<float>(a_uHeight);
	glViewport(0, 0, static_cast<GLint>(a_uWidth), static_cast<GLint>(a_uHeight));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/// \todo hard coded value, but it will be camera class value
	gluPerspective(45.0f, fRatio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}