#include "textureManager.h"

#include "BurgerEngine/Graphics/AbstractTexture.h"
#include "BurgerEngine/Graphics/Texture2D.h"
#include "BurgerEngine/Graphics/TextureCubeMap.h"
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

Texture2D* TextureManager::getTexture2D(const std::string& name, const bool useMipmap)
{
	//Check if the texture is already loaded or not
	std::map<std::string , AbstractTexture*>::iterator iter = m_mTextures.find(name);
	if (iter == m_mTextures.end())
	{
		Texture2D* pTex = new Texture2D(useMipmap);
		if (!pTex->loadTexture(name))
		{
			std::cerr << "WARNING: cannot load texture: " << name << std::endl;
			return NULL;
		}
		m_mTextures[name] = pTex;
	}
	return static_cast<Texture2D*>(m_mTextures[name]);
}

TextureCubeMap* TextureManager::getTextureCubeMap( const std::string& sName )
{
	std::map<std::string , AbstractTexture*>::iterator iter = m_mTextures.find( sName );
	if (iter == m_mTextures.end())
	{
		TextureCubeMap* pTex = new TextureCubeMap();
		if (!pTex->loadTexture( sName ) )
		{
			std::cerr << "WARNING: cannot load texture: " << sName << std::endl;
			return NULL;
		}
		m_mTextures[ sName ] = pTex;
	}
	return static_cast<TextureCubeMap*>(m_mTextures[ sName ]);
}