/*************************************
*
*		BurgerEngine Project
*		
*		Created :	19/09/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/


#ifndef __SCENEGRAPH_H__
#define __SCENEGRAPH_H__

#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/Graphics/SceneLight.h"

#include "BurgerEngine/GUI/DebugMenu.h"

#include <vector>
#include <map>

class AbstractMesh;
class SceneMesh;
class OmniLight;
class SpotLight;
class SpotShadow;
class SkyBox;

/// \class	SceneGraph
/// \brief	Used to display 3D scenes
class SceneGraph
{
public:

	/// \brief Constuctor, call parent
	SceneGraph();
	~SceneGraph();

	bool Init();

	const std::vector< SceneMesh* >& GetSceneMeshes() const { return m_oSceneMeshes; };
	
	const std::vector< SceneMesh* >& GetTransparentSceneMeshes() const { return m_oTransparentSceneMeshes; };
	std::vector< SceneMesh* >& GetTransparentSceneMeshes() { return m_oTransparentSceneMeshes; };
	
	const std::vector< OmniLight* >& GetOmniLights() const { return m_oOmniLights; };
	
	const std::vector< SpotLight* >& GetSpotLights()const { return m_oSpotLights; };
	
	const std::vector< SpotShadow* >& GetSpotShadows() const { return m_oSpotShadows; };

	const SkyBox* GetSkyBox() const { return m_pSkyBox; };

	const DebugMenu& GetDebugMenu() const { return m_oDebugMenu; };
	DebugMenu& GetDebugMenu() { return m_oDebugMenu; };

private:
/// \brief Clears object and light lists
	void Clear();
/// \brief Loads objects, light etc. from a XML file
	void LoadSceneXML( const char * sName );

private:
	///lists of renderable objects and lights
	std::vector< SceneMesh* >		m_oSceneMeshes;
	std::vector< SceneMesh* >		m_oTransparentSceneMeshes;
	std::vector< OmniLight* >		m_oOmniLights;
	std::vector< SpotLight* >		m_oSpotLights;
	std::vector< SpotShadow* >		m_oSpotShadows;

	SkyBox *						m_pSkyBox;

	std::map< std::string, SceneLight::LightType > m_oStringToLightTypeMap;

	DebugMenu m_oDebugMenu;
};


#endif //__SCENEGRAPH_H__
