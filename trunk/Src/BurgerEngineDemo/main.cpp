//Very dirty testing main

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
#include "BurgerEngine/Core/StageManager.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Input/SFMLInputManager.h"


// Origine de la fenetre
static const unsigned int windowx = 200;
static const unsigned int windowy = 200;

// Dimensions de la fenetre
static unsigned int windowwidth  = 640;
static unsigned int windowheight = 360;


float ouverture = 45.0f;


//Timer variables
static unsigned int timebaseFPS;
static unsigned int timebase;
static unsigned int currentTime;
static unsigned int frames = 0;

GLenum err;

#define CHECK_ERRORS

static void reshapeGL(int newwidth, int newheight);

static void displayGL(void)
{
	//Reshape to normal
	reshapeGL(windowwidth, windowheight);

	//StageManager::GetInstance().Display();

	glutSwapBuffers();
	++frames;
}

static void reshapeGL(int newwidth, int newheight)
{
	float ratio = (float)newwidth/(float)newheight;
	glViewport(0, 0, (GLint)newwidth, (GLint)newheight);
  
	windowwidth = newwidth;
	windowheight = newheight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(ouverture, ratio, 0.5, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

static void idleGL(void)
{

	///Debug Mode : Check for OpenGL error
	#ifdef CHECK_ERRORS
		err = glGetError();
		if(err!=GL_NO_ERROR){
			std::cerr << "Erreur GL :" << std::endl;
			std::cerr << gluErrorString(err) << std::endl;
		}
	#endif

	///TODO: class timer
	currentTime= glutGet(GLUT_ELAPSED_TIME);
	///Show FPS
	if((currentTime- timebaseFPS) >= 1000)
	{
		std::cout<<"fps : "<< frames*1000.0f/(currentTime- timebaseFPS)<<std::endl;
		frames = 0;
		timebaseFPS = currentTime;
	}
	///Block to 60 FPS
	if((currentTime- timebase) >= 15)
	{

		/*glutPostRedisplay();
		
		timebase = currentTime;
		StageManager::GetInstance().Update();
		///If the scene is over, we move on to the next scene
		if (StageManager::GetInstance().IsFinished())
		{
			StageManager::GetInstance().SetCurrentStage(StageManager::GetInstance().GetNextStageId());
			StageManager::GetInstance().Reset();
		}

	
	*/
	}



}






static void initGL(int argc, char **argv)
{

	///Initialisation of Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error : " << glewGetErrorString(err) << std::endl;
		return ;
	}

	glClearColor(1.0f,1.0f,1.0f,1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

  
	//if(!isExtensionSupported("GL_ARB_shading_language_100"))
	//	quit();

	///timer initialisation
	timebase = glutGet(GLUT_ELAPSED_TIME);
	timebaseFPS = glutGet(GLUT_ELAPSED_TIME);


	glutSetCursor(GLUT_CURSOR_NONE);

	
}

static void initMesh()
{

}

int main(int argc, char **argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowPosition(windowx, windowy);
  glutInitWindowSize(windowwidth, windowheight);



 // glutReshapeFunc(reshapeGL);
//  glutDisplayFunc(displayGL);
  glutIdleFunc(idleGL);

 

  Engine::GetInstance().Init();



  Engine::GetInstance().Run();

  //if we exit the run loop it's over
  Engine::KillInstance();
  return 0 ;
}

