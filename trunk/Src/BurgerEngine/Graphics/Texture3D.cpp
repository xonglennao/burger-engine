#include "BurgerEngine/Graphics/Texture3D.h"
#include "BurgerEngine/Graphics/ImageTool.h"

Texture3D::Texture3D()
	: AbstractTexture()
{
}

Texture3D::Texture3D ( unsigned int iWidth, unsigned int iHeight, unsigned int iDepth, bool bUseMipMaps, bool bLinearFiltering, bool bClampS, bool bClampT, bool bClampR )
	: AbstractTexture( bUseMipMaps, bLinearFiltering, bClampS, bClampT )
	, m_eWrapR( GL_REPEAT )
	, m_iWidth( iWidth )
	, m_iHeight( iHeight )
	, m_iDepth( iDepth ) 
{
	if( bClampR )
	{
		m_eWrapR = GL_CLAMP_TO_EDGE;
	}
}

GLubyte* Texture3D::loadData()
{
	return ImageTool::load3DFrom2DImage( m_sName, m_iWidth, m_iHeight, m_iDepth, m_iChanel );
}

bool Texture3D::loadTexture(const std::string &name)
{
	m_sName = name;
    GLubyte *data = NULL; 
	data = loadData();

	if (data == NULL)
	{
		return false;
	}
	
	///Create a new ID
	destroyID();
	generateID();

	Activate();

	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, m_eGenerateMipMap );

	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, m_eFilteringMin );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, m_eFilteringMag );

	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, m_eWrapS );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, m_eWrapT );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, m_eWrapR );

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, m_iWidth, m_iHeight, m_iDepth, 0, GL_RGB, GL_UNSIGNED_BYTE , data );
	
	Deactivate();
	delete[] data;

	return true;

}

void Texture3D::Activate()
{
	//OpenGL Bind
	glBindTexture(GL_TEXTURE_3D,m_iId);
}

void Texture3D::Deactivate()
{
	glBindTexture(GL_TEXTURE_3D,0);
}