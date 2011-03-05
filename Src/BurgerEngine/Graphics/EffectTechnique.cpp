#include "EffectTechnique.h"
#include "BurgerEngine/Graphics/Shader.h"
#include "BurgerEngine/Graphics/AbstractTexture.h"
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

void EffectTechnique::CommitUniforms()
{
	std::map< int, float >::iterator FloatIt = m_oUniformFloatsMap.begin();
	while( FloatIt != m_oUniformFloatsMap.end() )
	{
		m_pShader->setUniformf( (*FloatIt).first, (*FloatIt).second );
		++FloatIt;
	}
	std::map< int, vec4 >::iterator Vec4It = m_oUniformVec4Map.begin();
	while( Vec4It != m_oUniformVec4Map.end() )
	{
		m_pShader->setUniform4fv( (*Vec4It).first, 1, (*Vec4It).second );
		++Vec4It;
	}

}

void EffectTechnique::ActivateTextures()
{
	std::map< int, AbstractTexture* >::iterator it = m_oUniformTextures2DMap.begin();
	while( it != m_oUniformTextures2DMap.end() )
	{
		glActiveTexture( GL_TEXTURE0 + (*it).first );
		(*it).second->Activate();
		++it;
	}
}