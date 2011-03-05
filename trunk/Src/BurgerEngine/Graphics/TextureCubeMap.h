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

#ifndef __TEXTURECUBEMAP_H__
#define __TEXTURECUBEMAP_H__

#include "abstractTexture.h"

class TextureCubeMap: public AbstractTexture
{

	public:

		TextureCubeMap ();
		TextureCubeMap( bool bUseMipMaps, bool bLinearFiltering, bool bClampS, bool bClampT, bool bClampR );
		virtual ~TextureCubeMap () {};

		/**
		*	@name loadTexture
		*	@brief load all the data
		*/
		virtual bool LoadTexture( const std::string &name );
		virtual void Activate();	
		virtual void Deactivate();	




	protected:
		GLenum	m_eWrapR;
		

};

#endif //__TEXTURECUBEMAP_H__
