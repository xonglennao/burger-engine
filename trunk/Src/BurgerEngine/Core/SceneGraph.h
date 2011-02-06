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

#include <vector>
#include <map>

class AbstractMesh;
class SceneMesh;
class OmniLight;
class SpotLight;
class SpotShadow;

/// \class	SceneGraph
/// \brief	Used to display 3D scenes
class SceneGraph
{
public:

	/// \brief Constuctor, call parent
	SceneGraph();
	~SceneGraph();

	bool Init();

	const std::vector< SceneMesh* >& GetSceneMeshes(){ return m_oSceneMeshes; };
	const std::vector< SceneMesh* >& GetTransparentSceneMeshes(){ return m_oTransparentSceneMeshes; };
	const std::vector< OmniLight* >& GetOmniLights(){ return m_oOmniLights; };
	const std::vector< SpotLight* >& GetSpotLights(){ return m_oSpotLights; };
	const std::vector< SpotShadow* >& GetSpotShadows(){ return m_oSpotShadows; };
private:
/// \brief Clears object and light lists
	void Clear();
/// \brief Loads objects, light etc. from a XML file
	void LoadSceneXML( const char * sName );

private:
	///lists of renderable objects and lights
	std::vector< SceneMesh* > m_oSceneMeshes;
	std::vector< SceneMesh* > m_oTransparentSceneMeshes;
	std::vector< OmniLight* > m_oOmniLights;
	std::vector< SpotLight* > m_oSpotLights;
	std::vector< SpotShadow* > m_oSpotShadows;

	std::map< std::string, SceneLight::LightType > m_eStringToLightTypeMap;
};


#endif //__SCENEGRAPH_H__
