/*************************************
*
*		BurgerEngine Project
*		
*		Created :	16/07/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/


#ifndef __PARTICLERENDERER_H__
#define __PARTICLERENDERER_H__

#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/Graphics/Vertex.h"
#include <map>
#include <vector>
#include <string>


typedef std::vector<Vertex> Vertexs;
typedef std::map<std::string, Vertexs> VertexMap;

/// \class ParticleRenderer
/// \brief Hold vector of vertex to be rendered each frame
class ParticleRenderer
{
public:

	/// \brief Draw every particle
	void Render();

	/// \brief Remove every vertex to display
	void CleanUp();
private:

	VertexMap m_mVertexs;

};

#endif // __PARTICLERENDERER_H__