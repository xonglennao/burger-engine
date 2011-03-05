#include "texture2D.h"

Texture2D::Texture2D()
	: AbstractTexture()
{
}

Texture2D::Texture2D ( bool bUseMipMaps, bool bLinearFiltering, bool bClampS, bool bClampT )
	: AbstractTexture( bUseMipMaps, bLinearFiltering, bClampS, bClampT )
{
}

bool Texture2D::LoadTexture( const std::string &name )
{
	m_sName = name;
    GLubyte *data = NULL; 
	data = AbstractTexture::LoadData();

	if (data == NULL)
	{
		return false;
	}
	
	///Create a new ID
	destroyID();
	generateID();

	Activate();
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_eFilteringMin );
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, m_eGenerateMipMap );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_eFilteringMag );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_eWrapS );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_eWrapT );

	//RGB or RGBA
	if (m_iChanel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE , data );
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE , data );
	
	Deactivate();
	delete[] data;

	return true;

}

void Texture2D::Activate()
{
	//OpenGL Bind
	glBindTexture(GL_TEXTURE_2D,m_iId);
}

void Texture2D::Deactivate()
{
	glBindTexture(GL_TEXTURE_2D,0);
}