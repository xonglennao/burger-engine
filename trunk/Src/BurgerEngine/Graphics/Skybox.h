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

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <vector>
#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/Graphics/SceneObject.h"
class Material;
struct vec3;
/// \name	SkyBox.h
/// \brief	A large textured cube used to simulate the sky

class SkyBox: public SceneObject
{
public:

	/// \brief Constructor
	/// \param[in] pMesh Pointer to the actual mesh?
	SkyBox( float fSize );
	~SkyBox(){}

public: 
	void Draw( const vec3& f3Position, unsigned int iFlag ) const;

	void SetMaterial( Material * pMaterial ){ m_pMaterial = pMaterial ; }

private:
	void BuildCube( float fSize );
	//the sphere has glitches, I need to debug it
	//void BuildSphere( float fSize, unsigned int iSlices, unsigned int iStacks );

private:	
	Material *	m_pMaterial;
	GLuint		m_iBufferID;
	GLuint		m_iBufferID2;
	unsigned int m_iVertexCount;
};

#endif //__SKYBOX_H__