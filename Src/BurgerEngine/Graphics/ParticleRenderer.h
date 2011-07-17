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
#include "BurgerEngine/Graphics/ParticleBatch.h"
#include <map>
#include <vector>
#include <string>


typedef std::vector<ParticleBatch> Vertexs;


/// \class ParticleRenderer
/// \brief Hold vector of vertex to be rendered each frame
class ParticleRenderer
{
public:

	/// \brief Draw every particle
	void Render();

	/// \brief Remove every vertex to display
	void CleanUp();

	ParticleBatch& RequestNewBatch();
private:

	Vertexs m_vVertexs;

};

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
inline ParticleBatch& ParticleRenderer::RequestNewBatch()
{
	ParticleBatch oBatch;
	m_vVertexs.push_back(oBatch);
	return m_vVertexs[m_vVertexs.size() - 1];
}
#endif // __PARTICLERENDERER_H__