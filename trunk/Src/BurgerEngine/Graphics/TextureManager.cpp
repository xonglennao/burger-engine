#include "textureManager.h"

#include "BurgerEngine/Graphics/AbstractTexture.h"
#include "BurgerEngine/Graphics/Texture2D.h"
#include "BurgerEngine/Graphics/TextureCubeMap.h"
#include "BurgerEngine/Graphics/Texture3D.h"

#include "BurgerEngine/External/TinyXml/TinyXml.h"

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

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void TextureManager::clear()
{
	for (std::map<std::string , AbstractTexture*>::iterator iter = m_mTextures.begin() ; iter != m_mTextures.end() ; ++iter)
	{
		delete (*iter).second;
	}
	m_mTextures.clear();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractTexture* TextureManager::GetTexture2D( const char * sName , const bool bUseMipmap, const bool bLinearFiltering, const bool bClampS, const bool bClampT )
{
	Texture2D* pTex = new Texture2D( bUseMipmap, bLinearFiltering, bClampS, bClampT );
	if ( !pTex->LoadTexture( sName ) )
	{
		std::cerr << "WARNING: cannot load texture: " << sName << std::endl;
		delete pTex;
		return NULL;
	}
	return pTex;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractTexture* TextureManager::GetTextureCubeMap( const char * sName , const bool bUseMipmap, const bool bLinearFiltering, const bool bClampS, const bool bClampT, const bool bClampR )
{
	TextureCubeMap* pTex = new TextureCubeMap( bUseMipmap, bLinearFiltering, bClampS, bClampT, bClampR );
	if ( !pTex->LoadTexture( sName ) )
	{
		std::cerr << "WARNING: cannot load texture: " << sName << std::endl;
		delete pTex;
		return NULL;
	}
	return pTex;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractTexture* TextureManager::GetTexture3DFrom2DFile( const char * sName, unsigned int iWidth, unsigned int iHeight, unsigned int iDepth, const bool bUseMipmap, const bool bLinearFiltering, const bool bClampS, const bool bClampT, const bool bClampR )
{
	Texture3D* pTex = new Texture3D( iWidth, iHeight, iDepth, bUseMipmap, bLinearFiltering, bClampS, bClampT, bClampR );
	if ( !pTex->LoadTexture( sName ) )
	{
		std::cerr << "WARNING: cannot load texture: " << sName << std::endl;
		delete pTex;
		return NULL;
	}
	return pTex;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractTexture* TextureManager::AddTexture( const char * sName )
{
	std::map<std::string , AbstractTexture*>::iterator iter = m_mTextures.find( sName );
	
	if ( iter == m_mTextures.end() )
	{
		TiXmlDocument * pDocument = new TiXmlDocument( sName );

		if( !pDocument->LoadFile() )
		{
  			std::cerr << "[ReadXML] Loading Error : " << pDocument->ErrorDesc() << std::endl;
			return NULL;
		}
	
		TiXmlElement * pRoot = pDocument->FirstChildElement( "texture" );

		if( pRoot )
		{
			int iUseMipMap = 0, iLinearFiltering = 0, iClampS = 0, iClampT = 0;
			pRoot->QueryIntAttribute( "mipmap", &iUseMipMap );
			pRoot->QueryIntAttribute( "linear", &iLinearFiltering );
			pRoot->QueryIntAttribute( "clamps", &iClampS );
			pRoot->QueryIntAttribute( "clampt", &iClampT );

			std::string sTextureType = pRoot->Attribute("type");
		
			TiXmlElement * pXmlFileName = pRoot->FirstChildElement( "filename" );
			if( pXmlFileName )
			{
				AbstractTexture * pTexture = NULL;
				if( sTextureType == "texture2D" )
				{
					pTexture = GetTexture2D( pXmlFileName->GetText(), iUseMipMap != 0 , iLinearFiltering != 0, iClampS != 0, iClampT != 0 );
				}
				else
				{
					int iClampR;
					pRoot->QueryIntAttribute( "clampr", &iClampR );
					if(sTextureType == "texturecube")
					{
						pTexture = GetTextureCubeMap( pXmlFileName->GetText(), iUseMipMap != 0 , iLinearFiltering != 0, iClampS != 0, iClampT != 0, iClampR != 0 );
					}
					else if(sTextureType == "texture3D")
					{
						int iWidth, iHeight, iDepth;
						pRoot->QueryIntAttribute( "width", &iWidth );
						pRoot->QueryIntAttribute( "height", &iHeight );
						pRoot->QueryIntAttribute( "depth", &iDepth );
						pTexture = GetTexture3DFrom2DFile( pXmlFileName->GetText(), iWidth, iHeight, iDepth,iUseMipMap != 0 , iLinearFiltering != 0, iClampS != 0, iClampT != 0, iClampR != 0 );
					}
				}
				if( pTexture )
				{
					return m_mTextures[ sName ] = pTexture;
				}
			}
		}
		return NULL;
	}
	return m_mTextures[ sName ];
}