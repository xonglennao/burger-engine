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

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <map>

#include "BurgerEngine/Graphics/EffectTechnique.h"

class Material
{
public:

	Material( const char * sName );
	~Material();

	bool Activate( EffectTechnique::RenderingTechnique eTechnique );
	void Desactivate( EffectTechnique::RenderingTechnique eTechnique );

private:

	void LoadMaterialXML( const char * sName );

private:		
	std::map< EffectTechnique::RenderingTechnique, EffectTechnique* > m_oTechniques;
};

#endif //__MATERIAL_H__