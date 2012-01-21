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
#include <stack>

#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/gui/DebugMenu.h"

///Forward declaration
class SceneMesh;
class DirectionalLight;
class OmniLight;
class SpotLight;
class SpotShadow;
class SceneLight;

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
	void Update( float fDeltaTime );

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

	const std::vector< DirectionalLight* >& GetDirectionalLights() const { return m_oDirectionalLights; }
	std::vector< DirectionalLight* >& GrabDirectionalLights(){ return m_oDirectionalLights; }

	const SkyBox* GetSkyBox() const { return m_pSkyBox; }
	void SetSkyBox( SkyBox* pSkyBox ) { m_pSkyBox = pSkyBox; }

	const DebugMenu& GetDebugMenu() const { return m_oDebugMenu; }
	DebugMenu& GetDebugMenu() { return m_oDebugMenu; }

	/// \brief Grabber
	DeferredRenderer& GrabRenderer(){return *m_pDeferredRenderer;}

	/// \brief Grab Particle renderer
	/// \todo make a vector of all renderer, and something like GrabRenderer<Type>
	ParticleRenderer& GrabParticleRenderer(){assert(m_pParticleRenderer);return *m_pParticleRenderer;}

	/// \brief Add a light, depending on it's type
	void AddLight(SceneLight& a_rLight, SceneLight::LightType a_eType);

	const float4x4& GetMVP() const {return m_oModelViewProjectionMatrices.top(); };
	void PushMVP(const float4x4& mMVP ){ m_oModelViewProjectionMatrices.push(mMVP); };
	void PopMVP(){ m_oModelViewProjectionMatrices.pop(); };

	//const float4x4& GetView() const {return m_oViewMatrices.top(); };
	//void PushView(const float4x4& mMVP ){ m_oViewMatrices.push(mMVP); };
	//void PopView(){ m_oViewMatrices.pop(); };
	
	const float4x4& GetModelView() const {return m_oModelViewMatrices.top(); };
	void PushModelView(const float4x4& mMVP ){ m_oModelViewMatrices.push(mMVP); m_mNormalMatrix = transpose(!mMVP); };
	void PopModelView(){ m_oModelViewMatrices.pop(); };

	void SetCurrentShader( Shader* pShader ){ m_pCurrentShader = pShader; }
	Shader* GetCurrentShader() { return m_pCurrentShader; }

	const float4x4& GetNormalMatrix() const { return m_mNormalMatrix; }

private:
	/// The actual renderer
	/// List of renderer? This will come with pipeline
	DeferredRenderer* m_pDeferredRenderer;

	ParticleRenderer* m_pParticleRenderer;

	///lists of renderable objects and lights
	/// \todo clear all this, 
	std::vector< SceneMesh* >			m_oSceneMeshes;
	std::vector< SceneMesh* >			m_oTransparentSceneMeshes;
	std::vector< OmniLight* >			m_oOmniLights;
	std::vector< SpotLight* >			m_oSpotLights;
	std::vector< SpotShadow* >			m_oSpotShadows;
	std::vector< DirectionalLight* >	m_oDirectionalLights;

	SkyBox *						m_pSkyBox;
	DebugMenu m_oDebugMenu;

	Shader * m_pCurrentShader;

	std::stack<float4x4> m_oModelViewProjectionMatrices;
	//std::stack<float4x4> m_oViewMatrices;
	std::stack<float4x4> m_oModelViewMatrices;
	float4x4 m_mNormalMatrix;
};


#endif //__RENDERINGCONTEXT_H__

