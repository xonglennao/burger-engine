#include "textureManager.h"

#include "BurgerEngine/Graphics/AbstractTexture.h"
#include "BurgerEngine/Graphics/Texture2D.h"
#include "BurgerEngine/Graphics/TextureCubeMap.h"
#include "BurgerEngine/Graphics/Texture3D.h"

TextureManager::TextureManager()
{
	//Devil initialization
	ilInit();

	// Upper left corner
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
	ilEnable(IL_ORIGIN_SET);

	ilEnable(IL_TYPE_SET);
	ilTypeFunc(IL_UNSIGNED_BYTE);
}

void TextureManager::clear()
{
	for (std::map<std::string , AbstractTexture*>::iterator iter = m_mTextures.begin() ; iter != m_mTextures.end() ; ++iter)
	{
		delete (*iter).second;
	}
	m_mTextures.clear();
}

Texture2D* TextureManager::getTexture2D( const std::string &sName, const bool bUseMipmap, const bool bLinearFiltering, const bool bClampS, const bool bClampT )
{
	//Check if the texture is already loaded or not
	std::map<std::string , AbstractTexture*>::iterator iter = m_mTextures.find(sName);
	if ( iter == m_mTextures.end() )
	{
		Texture2D* pTex = new Texture2D( bUseMipmap, bLinearFiltering, bClampS, bClampT );
		if (!pTex->loadTexture( sName ) )
		{
			std::cerr << "WARNING: cannot load texture: " << sName << std::endl;
			return NULL;
		}
		m_mTextures[sName] = pTex;
	}
	return static_cast< Texture2D* >( m_mTextures[sName] );
}

TextureCubeMap* TextureManager::getTextureCubeMap( const std::string& sName, const bool bUseMipmap, const bool bLinearFiltering, const bool bClampS, const bool bClampT, const bool bClampR )
{
	std::map<std::string , AbstractTexture*>::iterator iter = m_mTextures.find( sName );
	if (iter == m_mTextures.end())
	{
		TextureCubeMap* pTex = new TextureCubeMap( bUseMipmap, bLinearFiltering, bClampS, bClampT, bClampR );
		if (!pTex->loadTexture( sName ) )
		{
			std::cerr << "WARNING: cannot load texture: " << sName << std::endl;
			return NULL;
		}
		m_mTextures[ sName ] = pTex;
	}
	return static_cast<TextureCubeMap*>(m_mTextures[ sName ]);
}

Texture3D* TextureManager::getTexture3DFrom2DFile( const std::string& sName, unsigned int iWidth, unsigned int iHeight, unsigned int iDepth, const bool bUseMipmap, const bool bLinearFiltering, const bool bClampS, const bool bClampT, const bool bClampR )
{
	std::map<std::string , AbstractTexture*>::iterator iter = m_mTextures.find( sName );
	if (iter == m_mTextures.end())
	{
		Texture3D* pTex = new Texture3D( iWidth, iHeight, iDepth, bUseMipmap, bLinearFiltering, bClampS, bClampT, bClampR );
		if (!pTex->loadTexture( sName ) )
		{
			std::cerr << "WARNING: cannot load texture: " << sName << std::endl;
			return NULL;
		}
		m_mTextures[ sName ] = pTex;
	}
	return static_cast<Texture3D*>(m_mTextures[ sName ]);
}