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


#ifndef __STAGEMAINS3DCENE_H__
#define __STAGEMAINS3DCENE_H__

#include "BurgerEngine/Core/AbstractStage.h"
#include "BurgerEngine/Graphics/CommonGraphics.h"

#include <vector>

class AbstractMesh;
class SceneMesh;
class SceneLight;

/// \class	StageMain3DScene
/// \brief	Used to display 3D scenes
class StageMain3DScene: public AbstractStage
{
public:

	/// \brief Constuctor, call parent
	StageMain3DScene(std::string const& a_sId);
	~StageMain3DScene();

	bool Init();

	/// \brief Simple overriden update method
	void Update() {}

	///\ \brief Manage keyboard input
	bool OnKeyUp(unsigned char a_cKey);
	bool OnKeyDown(unsigned char a_cKey);

private:
	/// \brief No display OVERRIDE
	void _Render();

private:
	void	LoadSceneXML( const char * sName );

private:
	//lists of renderable object and light
	//should be replaced by a scenegraph?
	std::vector< SceneMesh* > m_oSceneMeshes;
	std::vector< SceneLight* > m_oSceneLights;

	//constants for lights, will be moved somewhere else
	GLfloat * m_pLightZeros;
	GLfloat * m_pLightDir;

};


#endif //__STAGEMAINS3DCENE_H__
