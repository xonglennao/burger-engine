/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/08/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __ABSTRACTTEXTURE_H__
#define __ABSTRACTTEXTURE_H__

#include <string>
#include <iostream>

#include "CommonGraphics.h"

class AbstractTexture
{

	public:

		AbstractTexture ();
		virtual ~AbstractTexture ();

		/**
		*	@name loadTexture
		*	@brief load all the data
		*/
		virtual bool loadTexture(const std::string& name) = 0;
		virtual bool loadTexture(GLenum target, const std::string& name) = 0;
		virtual void Activate() = 0;

		virtual GLubyte* loadData();
		void generateID();
		void destroyID();
	
	protected:

		std::string	m_sName;
		unsigned int	m_iWidth;
		unsigned int	m_iHeight;
		unsigned int	m_iChanel;
		GLuint			m_iId;
		

};

#endif //__ABSTRACTTEXTURE_H__
