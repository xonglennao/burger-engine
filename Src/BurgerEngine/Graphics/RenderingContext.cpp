#include "BurgerEngine/Graphics/RenderingContext.h"
#include "BurgerEngine/Graphics/DeferredRenderer.h"

#include "BurgerEngine/Graphics/SceneLight.h"
#include "BurgerEngine/Graphics/SpotLight.h"
#include "BurgerEngine/Graphics/OmniLight.h"
#include "BurgerEngine/Graphics/SpotShadow.h"

#include "BurgerEngine/Graphics/SceneMesh.h"
#include "BurgerEngine/Graphics/SkyBox.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
RenderingContext::RenderingContext():
	m_pSkyBox(NULL)
{

}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool RenderingContext::Initialize()
{
	m_pDeferredRenderer = new DeferredRenderer();
	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool RenderingContext::Terminate()
{
	delete m_pDeferredRenderer;
	delete m_pSkyBox;

	//All the mesh will be deleted in the components themselves
	//We only clear the pointer
	/*
	std::vector< SceneMesh* >::iterator oMeshIt = m_oSceneMeshes.begin();
	while( oMeshIt != m_oSceneMeshes.end() )
	{
	delete (*oMeshIt);
	(*oMeshIt) = NULL;
	++oMeshIt;
	}
	*/

	m_oSceneMeshes.clear();
	m_oTransparentSceneMeshes.clear();
	m_oOmniLights.clear();
	m_oSpotLights.clear();
	m_oSpotShadows.clear();
	m_oDirectionalLights.clear();


	return true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderingContext::Update()
{

	m_pDeferredRenderer->Render();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool RenderingContext::AddMesh(SceneMesh& a_rSceneMesh)
{
	//We shoudl have different pipeline to add the mesh to
	// It should not be 'IsTransparent" or IsOpaque" but more like
	// a rendering type?
	if (a_rSceneMesh.IsTransparent())
	{
		m_oTransparentSceneMeshes.push_back(&a_rSceneMesh);
	}
	else if (a_rSceneMesh.IsOpaque())
	{
		m_oSceneMeshes.push_back(&a_rSceneMesh);
	}

	return true;
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void RenderingContext::AddLight( SceneLight& a_rLight, SceneLight::LightType a_eType )
{
	switch (a_eType)
	{
		case SceneLight::E_DIRECTIONAL:m_oDirectionalLights.push_back(&a_rLight);break;
		case SceneLight::E_OMNI_LIGHT:m_oOmniLights.push_back(static_cast<OmniLight*>(&a_rLight));break;
		case SceneLight::E_SPOT_LIGHT: m_oSpotLights.push_back(static_cast<SpotLight*>(&a_rLight));break;
		case SceneLight::E_SPOT_SHADOW: m_oSpotShadows.push_back(static_cast<SpotShadow*>(&a_rLight));break;
		default:break;
	}
}


