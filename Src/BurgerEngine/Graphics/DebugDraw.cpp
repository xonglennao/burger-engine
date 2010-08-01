#include "DebugDraw.h"
#include <assert.h>


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawCube(osg::Vec3f const& a_rf3Position, float a_fSize)
{
	glDisable(GL_LIGHTING);

	glBegin(GL_QUADS);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);

		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, 1.0f);

		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);

		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f,  1.0f, -1.0f);
		glVertex3f(1.0f,  1.0f,  1.0f);
		glVertex3f(1.0f, -1.0f,  1.0f);

		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);

		glVertex3f(-1.0f, 1.0f,  1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f( 1.0f, 1.0f, -1.0f);
		glVertex3f( 1.0f, 1.0f,  1.0f);

	glEnd();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawAxis(osg::Vec3f const& a_rf3Position, float a_fSize)
{

	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(a_rf3Position.x(),a_rf3Position.y(),a_rf3Position.z());
		glVertex3f(a_rf3Position.x() + a_fSize,a_rf3Position.y(),a_rf3Position.z());

		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(a_rf3Position.x(),a_rf3Position.y(),a_rf3Position.z());
		glVertex3f(a_rf3Position.x(),a_rf3Position.y() + a_fSize,a_rf3Position.z());

		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(a_rf3Position.x(),a_rf3Position.y(),a_rf3Position.z());
		glVertex3f(a_rf3Position.x(),a_rf3Position.y(),a_rf3Position.z() + a_fSize);
	glEnd();

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DebugDraw::DrawGrid(float a_fSize, unsigned int a_uSubdivision)
{

	// We do note want to divide by zero
	assert(a_uSubdivision != 0);
	/// \todo a Float to in class
	float fSubsize = a_fSize / static_cast<float>(a_uSubdivision);

	glDisable(GL_LIGHTING);
	glColor3f(0.0f,0.0f,1.0f);

	// Back to wireframe
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	for(unsigned int i=0; i<a_uSubdivision-1; ++i)  // on dessine lignes par lignes
	{
		glBegin(GL_QUAD_STRIP);
		for(unsigned int j=0; j<a_uSubdivision; ++j)
		{
			glVertex3f(i*fSubsize, 0.0, j*fSubsize);
			glVertex3f((i+1)*fSubsize, 0.0, j*fSubsize);
		}
	
		glEnd();
	}

	// Back to fill mode
	/// \todo Shoudl go back to render mode set by engine
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

}