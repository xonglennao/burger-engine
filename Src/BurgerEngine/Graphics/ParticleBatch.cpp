#include "burgerEngine/Graphics/ParticleBatch.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
ParticleBatch::~ParticleBatch()
{
	delete m_pVerterBuffer;
	delete m_pIndexBuffer;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleBatch::Initialize(unsigned int a_uParticleCount)
{
	m_uParticleCount = a_uParticleCount;

	//Compute the base size of one particle vertex (pos + color + size )
	m_uParticleVertexSize = sizeof(vec3) + sizeof(vec4) + sizeof(vec2) ;
	
	//Check for optional parameters (uv + offset + rotation + direction)
	//this might come later, to make particle data lighter
	m_uParticleVertexSize += sizeof(vec2);
	//Offset
	//m_uParticleVertexSize += sizeof(vec2);
	//Rotation
	m_uParticleVertexSize += sizeof(vec2);
	
	// add the velocity for orientation
	//m_uParticleVertexSize += sizeof(gnNativeFloat3);
		


	//We have 4 vertex per particle, and 6 index
	m_uVertexBufferSize = a_uParticleCount * 4 * m_uParticleVertexSize;
	m_uIndexBufferSize = a_uParticleCount * 6* sizeof(IndexType);

	//We alloc all at once
	m_pVerterBuffer = new ByteType[m_uVertexBufferSize];
	m_pIndexBuffer = new IndexType[m_uIndexBufferSize];

	m_pCurrentVertexOffset = m_pVerterBuffer;

	m_uCurrentIndexCount = 0;
	m_uCurrentIndexIndice = 0;

	m_bInit = true;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleBatch::AddParticle(Particle const& a_rParticle)
{
	//Check if init
	assert(m_bInit);

		// Data
	_CopyParticleData(a_rParticle);	


	// Step for next particle
	++m_uParticleCount; 
	

	// Index
	_AddIndex(m_uCurrentIndexIndice);
	_AddIndex(m_uCurrentIndexIndice + 1);
	_AddIndex(m_uCurrentIndexIndice + 2);
	_AddIndex(m_uCurrentIndexIndice + 2);
	_AddIndex(m_uCurrentIndexIndice + 1);
	_AddIndex(m_uCurrentIndexIndice + 3);

	m_uCurrentIndexIndice += 4;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void ParticleBatch::_CopyParticleData(Particle const& a_rParticle)
{
	//Check if allocated
	assert(m_pCurrentVertexOffset);
	Bytes pData = m_pCurrentVertexOffset;

	
	//Rotation
	//Put in a float 2 in order to send it througth texture coordinates
	//Use the Sin cos to rotate the particle inside the shader
	float fSin = sinf(a_rParticle.fRotation);
	float fCos = cosf(a_rParticle.fRotation);
	vec2 f2Rotation(fSin, fCos);
	

	// Displacement vector
	vec2 f2Displacement[4];
	f2Displacement[0] = a_rParticle.f2Size * (- 1.0f);
	f2Displacement[1] = vec2(a_rParticle.f2Size.x, a_rParticle.f2Size.y);
	f2Displacement[2] = vec2(a_rParticle.f2Size.y, -a_rParticle.f2Size.y);
	f2Displacement[3] = a_rParticle.f2Size;


	// For each four vertex----
	for(unsigned int i = 0; i< 4;++i)
	{
		
		memcpy(pData, &(a_rParticle.f3Position) ,sizeof(vec3));
		pData += sizeof(vec3);

		memcpy(pData, &(a_rParticle.f4Color), sizeof(vec4));
		pData += sizeof(vec4);
		
		memcpy(pData, &(f2Displacement[i]) ,sizeof(vec2));
		pData += sizeof(vec2);

		//Optional, but for not we put everything
		memcpy(pData, &(a_rParticle.aUVs[i]) ,sizeof(vec2));
		pData += sizeof(vec2);

		//memcpy(pData, &(a_rParticle.f2OffSet) ,sizeof(vec2));
		//pData += sizeof(vec2);

		memcpy(pData, &(a_rParticle.fRotation) ,sizeof(vec2));
		pData += sizeof(vec2);


		/*if (m_fParticleParameters.GetFlag(gnGfxParticleBatchParametersDeclare::eUVs))
		{
			gnMemCopy(pData, &(raf2Uv[i]) ,sizeof(gnFloat2));
			pData += sizeof(gnFloat2);
		}

		if (m_fParticleParameters.GetFlag(gnGfxParticleBatchParametersDeclare::eOffset))
		{
			gnMemCopy(pData, pOffset ,sizeof(gnFloat2));
			pData += sizeof(gnFloat2);
		}

		if (m_fParticleParameters.GetFlag(gnGfxParticleBatchParametersDeclare::eRotation))
		{
			gnMemCopy(pData, &(f2Rotation) ,sizeof(gnFloat2));
			pData += sizeof(gnFloat2);
		}

		switch(m_eParticleMode)
		{
		case gnGfxParticleBatchModeDeclare::eAxisAligned:
			break;

		case gnGfxParticleBatchModeDeclare::eOriented:
			break;

		case gnGfxParticleBatchModeDeclare::eCameraAligned:
		default:
			break;

		}
		*/
		m_pCurrentVertexOffset += m_uParticleVertexSize;
	}
}