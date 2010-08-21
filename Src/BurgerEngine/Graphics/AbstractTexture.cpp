#include "AbstractTexture.h"
#include "ImageTool.h"

AbstractTexture::AbstractTexture()
{
}

AbstractTexture::~AbstractTexture()
{
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
