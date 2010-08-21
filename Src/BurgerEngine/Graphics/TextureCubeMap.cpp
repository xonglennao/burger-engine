#include "textureCubeMap.h"

TextureCubeMap::TextureCubeMap():
AbstractTexture()
{
	m_bFirstInit = false;
}
bool TextureCubeMap::loadTexture(GLenum target,const std::string &name)
{
	m_sName = name;
    GLubyte *data = NULL; 
	
	///Create a new ID
	if (!m_bFirstInit)
	{
		destroyID();
		generateID();
		m_bFirstInit = true;
	}

	activate();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	data = AbstractTexture::loadData();
	if (data != NULL)
	{
		//0x80E0 should be GL_BGR but I can't #include <glew.h>.
		glTexImage2D (target,0, GL_RGBA,m_iWidth, m_iHeight, 0, 0x80E0,GL_UNSIGNED_BYTE, data);
		//glTexImage2D (target,0, GL_RGBA,m_iWidth, m_iHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
	else
		return false;


	
	///We are loading the six faces, but we shoud use a LoaderTool instead of a HACK
	//NEGATIVE_X
	/*data = AbstractTexture::loadData();
	if (data != NULL)
	{
		glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0, GL_RGBA,m_iWidth, m_iHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
	else
		return false;

	//NEGATIVE_Y
	data = AbstractTexture::loadData();
	if (data != NULL)
	{
		glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0, GL_RGBA,m_iWidth, m_iHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
	else
		return false;

	//NEGATIVE_Z
	data = AbstractTexture::loadData();
	if (data != NULL)
	{
		glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0, GL_RGBA,m_iWidth, m_iHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
	else
		return false;

	//POSITIVE_X
	data = AbstractTexture::loadData();
	if (data != NULL)
	{
		glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X,0, GL_RGBA,m_iWidth, m_iHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
	else
		return false;

	//POSITIVE_Y
	data = AbstractTexture::loadData();
	if (data != NULL)
	{
		glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0, GL_RGBA,m_iWidth, m_iHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
	else
		return false;

	//POSITIVE_Z
	data = AbstractTexture::loadData();
	if (data != NULL)
	{
		glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0, GL_RGBA,m_iWidth, m_iHeight, 0, GL_RGB,GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
	else
		return false;*/
	
	desactivate();

	return true;

}

bool TextureCubeMap::loadTexture(const std::string &name)
{
	return loadTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,name);
}

void TextureCubeMap::activate()
{
	if (!m_bFirstInit)
		std::cout<<"INFO : The CubeMap has not been texturized"<<std::endl;

	desactivate();
	//OpenGL Bind
	glEnable (GL_TEXTURE_GEN_S);
	glEnable (GL_TEXTURE_GEN_T);
	glEnable (GL_TEXTURE_GEN_R);

	glEnable (GL_TEXTURE_CUBE_MAP);
	glBindTexture (GL_TEXTURE_CUBE_MAP, m_iId);

	glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
}

void TextureCubeMap::desactivate()
{
	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
	glDisable (GL_TEXTURE_CUBE_MAP);

	glDisable (GL_TEXTURE_GEN_S);
	glDisable (GL_TEXTURE_GEN_T);
	glDisable (GL_TEXTURE_GEN_R);
}