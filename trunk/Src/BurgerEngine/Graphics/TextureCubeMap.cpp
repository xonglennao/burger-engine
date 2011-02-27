#include "BurgerEngine/Graphics/TextureCubeMap.h"

const std::string oTextureName[6] = { "PosX", "NegX", "PosY", "NegY", "PosZ", "NegZ" };
/*
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X                     0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X                     0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y                     0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y                     0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z                     0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z                     0x851A
*/

TextureCubeMap::TextureCubeMap()
	: AbstractTexture()
{
}

TextureCubeMap::TextureCubeMap(  bool bUseMipMaps, bool bLinearFiltering, bool bClampS, bool bClampT, bool bClampR )
	: AbstractTexture( bUseMipMaps, bLinearFiltering, bClampS, bClampT )
	,  m_eWrapR( GL_REPEAT )
{
	if( bClampR )
	{
		m_eWrapR = GL_CLAMP_TO_EDGE;
	}
}

bool TextureCubeMap::loadTexture( const std::string &sName )
{
	GLubyte *data = NULL; 
	
	///Create a new ID
	generateID();

	Activate();

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, m_eGenerateMipMap );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_eFilteringMin );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_eFilteringMag );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_eWrapS );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_eWrapT );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_eWrapR );

	GLenum eTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X;

	for( unsigned int i = 0; i < 6; ++i )
	{
		m_sName = sName + oTextureName[i] + ".png";
		data = AbstractTexture::loadData();
		if (data != NULL)
		{
			glTexImage2D ( eTarget, 0, GL_RGB,m_iWidth, m_iHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data );
			delete[] data;
			eTarget++;
		}
		else
			return false;
	}
	
	Deactivate();

	return true;
}

void TextureCubeMap::Activate()
{
	//OpenGL Bind
	glBindTexture ( GL_TEXTURE_CUBE_MAP, m_iId );
}

void TextureCubeMap::Deactivate()
{
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
}