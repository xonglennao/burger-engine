#include "BurgerEngine/Graphics/SpotShadow.h"
#include "BurgerEngine/Graphics/FBO.h"

SpotShadow::SpotShadow()
{
	m_oDepth = new FBO( 640, 640, FBO::E_FBO_2D );
	m_oDepth->GenerateColorOnly( GL_RGB32F_ARB, GL_RGB );
}

SpotShadow::~SpotShadow()
{
}

void SpotShadow::ActivateBuffer(){ m_oDepth->Activate(); }
void SpotShadow::DesactivateBuffer(){ m_oDepth->Desactivate(); }
void SpotShadow::ActivateDepthTexture(){ m_oDepth->ActivateTexture(); }