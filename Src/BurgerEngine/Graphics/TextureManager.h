/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/08/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

#include "BurgerEngine/Base/Singleton.h"
#include "BurgerEngine\External\IL\il.h"

#include <map>
#include <string>
#include <assert.h>

//Foward Declaration to save compilation time
class AbstractTexture;
class Texture2D;
class TextureCubeMap;

class TextureManager: public Singleton<TextureManager>
{
	friend class Singleton<TextureManager>;


	//class 
	public:

		/**
		*	@name	Texture2D* getTexture2D(const &std::string name);
		*	@brief	get the texture2D, and load it if we need to.
		*/
		Texture2D* getTexture2D(const std::string &name, const bool useMipmap =  true);

		/**
		*	@name	TextureCubeMap& getTextureCubeMap(const &std::string name);
		*	@brief	get the texturefor a Cube Map, and load it if we need to.
		*/
		TextureCubeMap* getTextureCubeMap(const std::string &name);
		
		/**
		*	@name	void clear();
		*	@brief	Clear all the maps
		*/
		void clear();

	protected:
		std::map<std::string,AbstractTexture*>		m_mTextures;
		//std::map<std::string,int/*TextureCUBE* class*/>	m_mTexturesCubeMap;

		TextureManager();
		virtual ~TextureManager(){clear();};
};


#endif //__TEXTUREMANAGER_H__