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

#include "BurgerEngine/Graphics/CommonGraphics.h"

class AbstractTexture
{

	public:

		AbstractTexture ();
		AbstractTexture( bool bUseMipMaps, bool bLinearFiltering, bool bClampS, bool bClampT );
		virtual ~AbstractTexture ();

		/**
		*	@name loadTexture
		*	@brief load all the data
		*/
		virtual bool LoadTexture(const std::string& name) = 0;
		virtual void Activate() = 0;

		virtual GLubyte* LoadData();
		void generateID();
		void destroyID();
	
	protected:

		std::string		m_sName;
		unsigned int	m_iWidth;
		unsigned int	m_iHeight;
		unsigned int	m_iChanel;
		GLuint			m_iId;
		
		GLenum			m_eFilteringMin;
		GLenum			m_eFilteringMag;
		GLenum			m_eWrapS;
		GLenum			m_eWrapT;
		GLenum			m_eGenerateMipMap;
};

#endif //__ABSTRACTTEXTURE_H__
