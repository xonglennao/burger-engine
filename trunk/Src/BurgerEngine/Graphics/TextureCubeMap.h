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
		virtual ~TextureCubeMap () {};

		/**
		*	@name loadTexture
		*	@brief load all the data
		*/
		
		virtual bool loadTexture(GLenum target,const std::string &name);
		virtual bool loadTexture(const std::string &name);
		virtual void Activate();	
		virtual void Desactivate();	




	protected:

		bool m_bFirstInit;
		

};

#endif //__TEXTURECUBEMAP_H__
