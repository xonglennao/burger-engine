/*************************************
*
*		BurgerEngine Project
*		
*		Created :	05/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __PARTICLEBATCH_H__
#define __PARTICLEBATCH_H__

#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/FX/Particle.h"

#include <vector>

typedef GLubyte ByteType;
typedef ByteType* Bytes;
typedef unsigned short int IndexType; 
typedef IndexType* Indexs; 

/// \brief Use to hold data for display a bunch of particle
/// 1 particle batch = 1 draw call
class ParticleBatch
{
public:
	ParticleBatch():
		m_pVerterBuffer(NULL),
		m_pIndexBuffer(NULL),
		m_pCurrentVertexOffset(NULL),
		m_pMaterial(NULL),
		m_bInit(false)
		{}
	~ParticleBatch();

	void Initialize(unsigned int a_uParticleCount);

	void AddParticle(Particle const& a_rParticle);

	void SetMaterial(Material* a_pMaterial){m_pMaterial = a_pMaterial;}
	Material* GrabMaterialPtr(){return m_pMaterial;}

	Bytes const GetVertexBuffer() const {return m_pVerterBuffer;}
	Indexs const GetIndexBuffer() const {return m_pIndexBuffer;}

	unsigned int GetVertexBufferSize() const {return m_uVertexBufferSize;}
	unsigned int GetIndexCount() const {return m_uCurrentIndexCount;}
	unsigned int GetParticleVertexSize() const {return m_uParticleVertexSize;}
private:
	void _CopyParticleData(Particle const& a_rParticle);
	void _AddIndex(unsigned int a_uIndex);

private:
	
	/// VertexBuffer
	Bytes m_pVerterBuffer;
	/// Raw data of the Index buffer
	Indexs m_pIndexBuffer;
	/// Size of the buffers;
	unsigned int m_uVertexBufferSize;
	unsigned int m_uIndexBufferSize;
	/// Current offset for both buffer
	Bytes m_pCurrentVertexOffset;
	///ParticleCount
	unsigned int m_uParticleCount;
	///Size of one particle
	unsigned int m_uParticleVertexSize;


	unsigned int m_uCurrentIndexCount;
	unsigned int m_uCurrentIndexIndice;

	/// Pointer to the material
	Material* m_pMaterial;

	bool m_bInit;
	//Indexs m_pCurrentIndexOffset;

};


//----------------------------------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------------------------------
inline void ParticleBatch::_AddIndex(unsigned int a_uIndex)
{
	assert(m_pIndexBuffer);
	m_pIndexBuffer[m_uCurrentIndexCount] = static_cast<IndexType>(a_uIndex);
	++m_uCurrentIndexCount;
}

#endif // __PARTICLEBATCH_H__