/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/02/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __TEXTURE3D_H__
#define __TEXTURE3D_H__

#include "abstractTexture.h"

class Texture3D: public AbstractTexture
{

	public:

		Texture3D ();
		Texture3D ( unsigned int iWidth, unsigned int iHeight, unsigned int iDepth, bool bUseMipMaps, bool bLinearFiltering, bool bClampS, bool bClampT, bool bClampR );
		virtual ~Texture3D () {};

		virtual bool LoadTexture(const std::string &name);
		virtual void Activate();	
		
		virtual GLubyte* LoadData();
		
		static void Deactivate();

	protected:
		GLenum			m_eWrapR;
		unsigned int	m_iWidth;
		unsigned int	m_iHeight;
		unsigned int	m_iDepth; 

};

#endif //__TEXTURE3D_H__
