/*************************************
*
*		BurgerEngine Project
*		
*		Created :	19/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __STATICMESH_H__
#define __STATICMESH_H__

#include "AbstractMesh.h"
#include "BurgerEngine/External/Osg/Vec3"
#include "BurgerEngine/External/Osg/Vec2"
#include <vector>

/// \class StaticMesh
/// \brief Static Mesh, so far .obj
class StaticMesh: public AbstractMesh 
{

public:

	/// \brief default constructor
	StaticMesh();

	/// \brief Destructor
	~StaticMesh(){Destroy();}


	/// \brief Load a mesh and store value in the buffer
	bool LoadMesh(std::string const& a_sFilename);

	/// \brief Render the mesh
	void Render();

	/// \brief Free buffer
	void Destroy();

private:

	/// \brief Load the file
	/// \param[in] a_sFilename File to load
	bool LoadFromFile(std::string const& a_sFilename);

	/// \brief Compute normal from the model
	void ComputeNormals();

	/// \brief Compute trangent from the model
	void ComputeTangents();

	/// \brief Compute Bounding Box
	/// \todo Create a class bounding box
	void ComputeBoundingBox();

	/// \brief Fill buffer
	bool BuildBuffer();

	void Bind();
	void Unbind();

	/// Total number of vertex
	unsigned int		m_iNbVertex;

	/// Id of the buffer
	unsigned int		m_iBufferId;

	/// Vertex buffer
	std::vector<osg::Vec3>	m_vf3Position;

	/// Normal buffer
	std::vector<osg::Vec3>	m_vf3Normal;

	/// Texture Coordinate buffer
	std::vector<osg::Vec2>	m_vf3Texcoord;

	/// Tangent buffer
	std::vector<osg::Vec3>	m_vf3Tangent;

	/// Size of those buffer
	GLsizeiptr m_iSizeVertex;
	GLsizeiptr m_iSizeNormal;
	GLsizeiptr m_iSizeTexture;
	GLsizeiptr m_iSizeTangent;

private:

	/// Triangle structure
	struct sMeshTriangle 
	{
		unsigned int ind[3];
	};

	/// Group of triangle structure
	struct sMeshGroup 
	{
		std::string					m_sName;
		long						m_lMaterial;
		std::vector<sMeshTriangle>	m_vsTriangle;
	};

private:

	/// Mesh group Collection
	std::vector<sMeshGroup>		m_vGroup;

};

#endif //__STATICMESH_H__
