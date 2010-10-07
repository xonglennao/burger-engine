#include "EffectTechnique.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/Texture2D.h"

EffectTechnique::EffectTechnique()
	: m_pShader( NULL )
{
}

EffectTechnique::~EffectTechnique()
{
};

void EffectTechnique::Activate()
{
	m_pShader->Activate();
	ActivateTextures();
	m_pShader->CommitStdUniforms();
	CommitUniformFloats();
}

void EffectTechnique::CommitUniformFloats()
{
	std::map< int, float >::iterator it = m_oUniformFloatsMap.begin();
	while( it != m_oUniformFloatsMap.end() )
	{
		m_pShader->setUniformf( (*it).first, (*it).second );
		++it;
	}

}

void EffectTechnique::ActivateTextures()
{
	std::map< int, Texture2D* >::iterator it = m_oUniformTextures2DMap.begin();
	while( it != m_oUniformTextures2DMap.end() )
	{
		glActiveTexture( GL_TEXTURE0 + (*it).first );
		(*it).second->Activate();
		++it;
	}
}