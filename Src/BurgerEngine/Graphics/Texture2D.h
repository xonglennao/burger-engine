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

#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include "abstractTexture.h"

class Texture2D: public AbstractTexture
{

	public:

		Texture2D ();
		Texture2D (bool bMipMaps);
		virtual ~Texture2D () {};

		virtual bool loadTexture(const std::string &name);
		virtual void Activate();	
		//virtual void Deactivate();	

		static void Deactivate();

	protected:
		bool m_bMipMaps;

};

#endif //__TEXTURE2D_H__
