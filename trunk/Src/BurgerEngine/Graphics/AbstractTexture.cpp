#include "AbstractTexture.h"
#include "ImageTool.h"

AbstractTexture::AbstractTexture()
{
}

AbstractTexture::AbstractTexture( bool bUseMipMaps, bool bLinearFiltering, bool bClampS, bool bClampT )
	: m_eFilteringMin( GL_NEAREST )
	, m_eFilteringMag( GL_NEAREST )
	, m_eWrapS( GL_REPEAT )
	, m_eWrapT( GL_REPEAT )
	, m_eGenerateMipMap( GL_FALSE )
{
	if( bLinearFiltering )
	{
		m_eFilteringMin = GL_LINEAR;
		m_eFilteringMag = GL_LINEAR;
	}
	if( bUseMipMaps )
	{
		m_eFilteringMin = GL_LINEAR_MIPMAP_LINEAR;
		m_eGenerateMipMap = GL_TRUE;
	}
	if( bClampS )
	{
		m_eWrapS = GL_CLAMP_TO_EDGE;
	}
	if( bClampT )
	{
		m_eWrapT = GL_CLAMP_TO_EDGE;
	}
}

AbstractTexture::~AbstractTexture()
{
	destroyID();
}

GLubyte* AbstractTexture::loadData()
{
	return ImageTool::loadImage(m_sName,m_iWidth,m_iHeight,m_iChanel);
}

void AbstractTexture::generateID()
{
	glGenTextures(1, &m_iId);
}
void AbstractTexture::destroyID()
{
	glDeleteTextures(1, &m_iId);
}
