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

/// \name	Material.h
/// \brief	

class Material
{
public:
	
	enum MaterialStatus
	{
		E_LOADED,
		E_LOADING_ERROR,
		E_SHADER_ERROR,
	};

	Material();
	~Material();

	MaterialStatus Initialize( const char * sName );

	bool Activate( EffectTechnique::RenderingTechnique eTechnique );
	void Deactivate( EffectTechnique::RenderingTechnique eTechnique );

	bool IsOpaque();
	bool IsTransparent();

private:

	MaterialStatus _LoadMaterialXML( const char * sName );

private:		
	std::map< EffectTechnique::RenderingTechnique, EffectTechnique* > m_oTechniques;
};

#endif //__MATERIAL_H__