#include "BurgerEngine/Graphics/SpotShadow.h"
#include "BurgerEngine/Graphics/FBO.h"

SpotShadow::SpotShadow()
{
	m_oDepth = new FBO( iShadowMapSize, iShadowMapSize, FBO::E_FBO_2D );
	m_oDepth->GenerateColorOnly( GL_RGBA32F_ARB, GL_RGBA );
}

SpotShadow::~SpotShadow()
{
}

void SpotShadow::ActivateBuffer(){ m_oDepth->Activate(); }
void SpotShadow::DesactivateBuffer(){ m_oDepth->Desactivate(); }
void SpotShadow::ActivateDepthTexture(){ m_oDepth->ActivateTexture(); }