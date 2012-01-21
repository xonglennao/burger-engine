#include "DirectionalLight.h"
#include "BurgerEngine/Graphics/FBO.h"

float DirectionalLight::pCascadeSizes[4] = {0.02f,0.03f,0.05f,0.1f};

DirectionalLight::DirectionalLight()
{
	m_oDepth = new FBO( iShadowMapSize*iCascadeCount, iShadowMapSize, FBO::E_FBO_2D );
	m_oDepth->GenerateDepthOnly();
	m_pViewProjMatrix = new float4x4[iCascadeCount];
}

DirectionalLight::~DirectionalLight()
{
	delete m_oDepth;
	m_oDepth = NULL;

	delete [] m_pViewProjMatrix;
	m_pViewProjMatrix = NULL;
}

void DirectionalLight::SetMatrix( const float4x4& mMatrix, unsigned int iIndex )
{
	iIndex = max(0,min(iIndex,iCascadeCount-1));
	m_pViewProjMatrix[iIndex] = mMatrix;
}

const float4x4& DirectionalLight::GetMatrix( unsigned int iIndex ) const
{
	iIndex = max(0,min(iIndex,iCascadeCount-1));
	return m_pViewProjMatrix[iIndex];
}

const float4x4* DirectionalLight::GetMatrices() const
{
	return m_pViewProjMatrix;
}

void DirectionalLight::ActivateBuffer(){ m_oDepth->Activate(); }
void DirectionalLight::DeactivateBuffer(){ m_oDepth->Deactivate(); }
void DirectionalLight::ActivateDepthTexture(){ m_oDepth->ActivateDepthTexture(); }