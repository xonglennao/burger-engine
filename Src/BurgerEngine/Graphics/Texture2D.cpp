#include "texture2D.h"

Texture2D::Texture2D():
AbstractTexture()
{
	m_bMipMaps = true;
}

Texture2D::Texture2D (bool bMipMaps):
AbstractTexture(),
m_bMipMaps(bMipMaps)
{

}

bool Texture2D::loadTexture(const std::string &name)
{
	m_sName = name;
    GLubyte *data = NULL; 
	data = AbstractTexture::loadData();

	if (data == NULL)
	{
		return false;
	}
	
	///Create a new ID
	destroyID();
	generateID();

	activate();
	
	if (!m_bMipMaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//RGB or RGBA
	if (m_iChanel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE , data );
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE , data );
	
	desactivate();
	delete[] data;

	return true;

}

void Texture2D::activate()
{
	//OpenGL Bind
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_iId);
	 
}

void Texture2D::desactivate()
{
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
}