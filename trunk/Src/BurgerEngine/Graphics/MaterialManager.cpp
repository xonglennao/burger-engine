#include "MaterialManager.h"
#include "BurgerEngine/Graphics/Material.h"

#include <iostream>

MaterialManager::MaterialManager()
{
	FillRenderingTechniqueMap();
}

void MaterialManager::FillRenderingTechniqueMap()
{
	oTechniqueNameToIdMap[ "render_gbuffer"] = EffectTechnique::E_RENDER_GBUFFER; 
	oTechniqueNameToIdMap[ "render_opaque"] = EffectTechnique::E_RENDER_OPAQUE;
	oTechniqueNameToIdMap[ "render_transparency"] = EffectTechnique::E_RENDER_TRANSPARENCY;
}

void MaterialManager::clear()
{
	for (std::map<std::string,Material*>::iterator iter = m_oMaterials.begin(); iter != m_oMaterials.end() ; ++iter)
		delete (*iter).second;
	m_oMaterials.clear();
}

Material*	MaterialManager::getMaterial(const std::string& name)
{
	std::map<std::string,Material*>::iterator iter = m_oMaterials.find(name);
	if (iter == m_oMaterials.end())
	{
		std::cerr << "WARNING : cannot Find Material :  " << name << std::endl;
		return NULL;
	}
	return (*iter).second;
}

Material * MaterialManager::addMaterial( const char * sName )
{
	std::map<std::string,Material*>::iterator iter = m_oMaterials.find( sName );
	if (iter == m_oMaterials.end())
	{
		Material* pMaterial	 =	new Material(sName);
		m_oMaterials[ sName ] =	pMaterial;
		return pMaterial;
	}
	else
	{
		return m_oMaterials[ sName ];
	}
}