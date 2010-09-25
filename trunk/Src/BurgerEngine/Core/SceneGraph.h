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

#include <vector>

class AbstractMesh;
class SceneMesh;
class SceneLight;

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
	const std::vector< SceneLight* >& GetSceneLights(){ return m_oSceneLights; };

private:
/// \brief Clears object and light lists
	void Clear();
/// \brief Loads objects, light etc. from a XML file
	void LoadSceneXML( const char * sName );

private:
	///lists of renderable objects and lights
	std::vector< SceneMesh* > m_oSceneMeshes;
	std::vector< SceneLight* > m_oSceneLights;
};


#endif //__SCENEGRAPH_H__
