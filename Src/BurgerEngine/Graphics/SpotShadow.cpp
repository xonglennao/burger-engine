#include "BurgerEngine/Graphics/SpotShadow.h"
#include "BurgerEngine/Graphics/FBO.h"

SpotShadow::SpotShadow()
{
	m_oDepth = new FBO( iShadowMapSize, iShadowMapSize, FBO::E_FBO_2D );
	m_oDepth->GenerateColorOnly( GL_R32F, GL_LUMINANCE );
}

SpotShadow::~SpotShadow()
{
}

void SpotShadow::ActivateBuffer(){ m_oDepth->Activate(); }
void SpotShadow::DeactivateBuffer(){ m_oDepth->Deactivate(); }
void SpotShadow::ActivateDepthTexture(){ m_oDepth->ActivateTexture(); }