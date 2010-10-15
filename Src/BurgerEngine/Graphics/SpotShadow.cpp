#include "BurgerEngine/Graphics/SpotShadow.h"
#include "BurgerEngine/Graphics/FBO.h"

SpotShadow::SpotShadow()
{
	m_oDepth = new FBO( 512, 512, FBO::E_FBO_2D );
	m_oDepth->GenerateDepthOnly();
}

SpotShadow::~SpotShadow()
{
}

void SpotShadow::ActivateBuffer(){ m_oDepth->Activate(); }
void SpotShadow::DesactivateBuffer(){ m_oDepth->Desactivate(); }
void SpotShadow::ActivateDepthTexture(){ m_oDepth->ActivateDepthTexture(); }