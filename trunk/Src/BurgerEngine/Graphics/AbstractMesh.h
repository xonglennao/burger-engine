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

#ifndef __ABSTRACTMESH_H__
#define __ABSTRACTMESH_H__

#include "BurgerEngine/Graphics/CommonGraphics.h"

/// \class AbstractMesh
/// \brief Interface for meshes
class AbstractMesh
{

public:

	/// \brief default constructor
	AbstractMesh(){}

	/// \brief Destructor
	virtual ~AbstractMesh(){}


	/// \brief Load a mesh and store value in the buffer
	virtual bool LoadMesh(std::string const& a_fFilename) = 0;

	/// \brief Render the mesh
	virtual void Render() = 0;

	/// \brief Free buffer
	virtual void Destroy() = 0;

protected:

private:


};

#endif //__ABSTRACTMESH_H__
