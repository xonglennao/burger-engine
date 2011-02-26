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
#include "BurgerEngine/External/IL/il.h"

#include <map>
#include <string>
#include <assert.h>

//Foward Declaration to save compilation time
class AbstractTexture;
class Texture2D;
class TextureCubeMap;
class Texture3D;

class TextureManager: public Singleton<TextureManager>
{
	friend class Singleton<TextureManager>;


	//class 
	public:

		/**
		*	@name	Texture2D* getTexture2D(const &std::string name);
		*	@brief	get the texture2D, and load it if we need to.
		*/
		Texture2D* getTexture2D( const std::string &sName, const bool bUseMipmap =  false, const bool bLinearFiltering = false, const bool bClampS = false, const bool bClampT = false );

		/**
		*	@name	TextureCubeMap& getTextureCubeMap(const &std::string name);
		*	@brief	get the texture for a Cube Map, and load it if we need to.
		*/
		TextureCubeMap* getTextureCubeMap( const std::string &sName, const bool bUseMipmap =  false, const bool bLinearFiltering = false, const bool bClampS = false, const bool bClampT = false, const bool bClampR = false );
		
		/**
		*	@name	Texture3D* getTexture3DFrom2DFile(const std::string &name);
		*	@brief	get a 3D texture from a 2D file (unwrapped 3D texture), and load it if we need to.
		*/
		Texture3D* getTexture3DFrom2DFile( const std::string &sName, unsigned int iWidth, unsigned int iHeight, unsigned int iDepth, const bool bUseMipmap =  false, const bool bLinearFiltering = false, const bool bClampS = false, const bool bClampT = false, const bool bClampR = false );
		
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