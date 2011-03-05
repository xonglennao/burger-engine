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
		Texture2D ( bool bUseMipMaps, bool bLinearFiltering, bool bClampS, bool bClampT );
		virtual ~Texture2D () {};

		virtual bool LoadTexture(const std::string &name);
		virtual void Activate();	
		//virtual void Deactivate();	

		static void Deactivate();
};

#endif //__TEXTURE2D_H__
