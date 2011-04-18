/*************************************
*
*		BurgerEngine Project
*		
*		Created :	08/03/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __RENDERINGCONTEXT_H__
#define __RENDERINGCONTEXT_H__

#include <vector>
#include <map>

#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/gui/DebugMenu.h"

///Forward declaration
class SceneMesh;
class OmniLight;
class SpotLight;
class SpotShadow;

/// \class RenderingContext
/// \brief This class hold all the mesh light and such
/// \todo This should become more like a pipeline type
class RenderingContext
{
public:

	/// \brief Constructor
	RenderingContext();

	/// \brief Destructor
	~RenderingContext(){}

	/// \brief Update the renderer which right now means display
	void Update();

	/// \brief Life cycle - Init
	bool Initialize();

	/// \brief Life cycle - Destroy everything
	bool Terminate();

	/// \brief Add a mesh to to be render
	bool AddMesh(SceneMesh& a_rSceneMesh/*MeshType*/);

	/// \brief Getters
	const std::vector< SceneMesh* >& GetSceneMeshes() const { return m_oSceneMeshes; }

	const std::vector< SceneMesh* >& GetTransparentSceneMeshes() const { return m_oTransparentSceneMeshes; }
	std::vector< SceneMesh* >& GetTransparentSceneMeshes() { return m_oTransparentSceneMeshes; }

	const std::vector< OmniLight* >& GetOmniLights() const { return m_oOmniLights; }

	const std::vector< SpotLight* >& GetSpotLights()const { return m_oSpotLights; }
	std::vector< SpotLight* >& GrabSpotLights() { return m_oSpotLights; }

	const std::vector< SpotShadow* >& GetSpotShadows() const { return m_oSpotShadows; }
	std::vector< SpotShadow* >& GrabSpotShadows(){ return m_oSpotShadows; }

	const std::vector< SceneLight* >& GetDirectionalLights() const { return m_oDirectionalLights; }
	std::vector< SceneLight* >& GrabDirectionalLights(){ return m_oDirectionalLights; }

	const SkyBox* GetSkyBox() const { return m_pSkyBox; }

	const DebugMenu& GetDebugMenu() const { return m_oDebugMenu; }
	DebugMenu& GetDebugMenu() { return m_oDebugMenu; }

	/// \brief Grabber
	DeferredRenderer& GrabRenderer(){return *m_pDeferredRenderer;}

	/// \brief Add a light, depending on it's type
	void AddLight(SceneLight& a_rLight, SceneLight::LightType a_eType);

private:
	/// The actual renderer
	/// List of renderer? This will come with pipeline
	DeferredRenderer* m_pDeferredRenderer;

	///lists of renderable objects and lights
	/// \todo clear all this, 
	std::vector< SceneMesh* >		m_oSceneMeshes;
	std::vector< SceneMesh* >		m_oTransparentSceneMeshes;
	std::vector< OmniLight* >		m_oOmniLights;
	std::vector< SpotLight* >		m_oSpotLights;
	std::vector< SpotShadow* >		m_oSpotShadows;
	std::vector< SceneLight* >		m_oDirectionalLights;

	SkyBox *						m_pSkyBox;
	DebugMenu m_oDebugMenu;

};


#endif //__RENDERINGCONTEXT_H__

