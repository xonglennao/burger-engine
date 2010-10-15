/*************************************
*
*		BurgerEngine Project
*		
*		Created :	14/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __OPENGLCONTEXT_H__
#define __OPENGLCONTEXT_H__

#include "CommonGraphics.h"

/// \name	OpenGLContext.h
/// \brief	With this class you can Init the GL param. So far the context is shared between all window
/// However in the futur it would be nice to choose whether we want a different context for each window or not

class OpenGLContext
{
public:

	/// \brief default constructor
	OpenGLContext();

	/// \brief Destructor
	~OpenGLContext(){}

	/// \brief	Initial set up of the OpenGl context
	bool Initialize( const unsigned int a_fWindowWidth, const unsigned int a_fWindowHeight );

	/// \brief Clear variable
	void Terminate();

	/// \brief	Upon resizing of the window
	bool ReshapeGl(unsigned int a_uWidth, unsigned int a_uHeight );
	bool Reshape(unsigned int a_uWidth, unsigned int a_uHeight, float a_fFOV, float a_fNear, float a_fFar );

	/// \brief	Check if the rendering context has errors of not and display a message
	/// \return true if error, false otherwise
	bool CheckError();

	/// \group Grabber
	/// \{

	/// \}

	/// \group Getter
	/// \{

	/// \}

	/// \group Setter
	/// \{

	/// \}

private:


};

#endif //__OPENGLCONTEXT_H__
