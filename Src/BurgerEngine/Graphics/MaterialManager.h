/*************************************
*
*		BurgerEngine Project
*		
*		Created :	26/09/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __MATERIALMANAGER_H__
#define __MATERIALMANAGER_H__

#include <string>
#include <map>

#include "BurgerEngine/Base/Singleton.h"

#include "BurgerEngine/Graphics/EffectTechnique.h"

class Material;

class MaterialManager:public Singleton<MaterialManager>
{
	friend class Singleton<MaterialManager>;

	public:
		Material*	getMaterial(const std::string& name);
		Material*	addMaterial( const char * sName );

		void clear();

		EffectTechnique::RenderingTechnique GetTechniqueID( std::string sName ){ return oTechniqueNameToIdMap[sName]; };

	protected:

		void FillRenderingTechniqueMap();
		
		MaterialManager();
		virtual ~MaterialManager(){clear();};

		std::map<std::string,Material*> m_oMaterials;
		std::map< std::string, EffectTechnique::RenderingTechnique > oTechniqueNameToIdMap;
};

#endif //__MATERIALMANAGER_H__