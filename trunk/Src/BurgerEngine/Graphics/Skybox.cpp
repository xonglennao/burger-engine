#include "BurgerEngine/Graphics/SkyBox.h"
#include "BurgerEngine/Graphics/Material.h"
#include "BurgerEngine/External/Math/Vector.h"

const int I_VERTEX_COUNT = 24;
const int I_SIZE_VERTEX = sizeof(vec3);
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SkyBox::SkyBox( float fSize )
{
	//BuildCube( fSize );
	//BuildSphere(fSize, 100, 100 );
	BuildCube( fSize );
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SkyBox::BuildCube( float fSize )
{
	glGenBuffers(1, &m_iBufferID );

	unsigned int iSizeVec3 =  I_VERTEX_COUNT * I_SIZE_VERTEX;

	vec3 * pVertex = new vec3[ I_VERTEX_COUNT ];
	
	//face 1
	pVertex[0] = vec3(- fSize,  fSize, - fSize );

	pVertex[1] = vec3(- fSize,- fSize,- fSize );

	pVertex[2] = vec3( fSize,- fSize,- fSize );

	pVertex[3] = vec3( fSize, fSize,- fSize );

	//face 2
	pVertex[4] = vec3(- fSize, fSize, fSize );

	pVertex[5] = vec3( fSize, fSize, fSize );

	pVertex[6] = vec3( fSize,- fSize, fSize );

	pVertex[7] = vec3(- fSize,- fSize, fSize );

	//face 3
	pVertex[8] = vec3(- fSize, fSize,- fSize );

	pVertex[9] = vec3(- fSize, fSize, fSize );

	pVertex[10] = vec3(- fSize,- fSize, fSize );

	pVertex[11] = vec3(- fSize,- fSize,- fSize );

	//face 4
	pVertex[12] = vec3( fSize, fSize, fSize );

	pVertex[13] = vec3( fSize, fSize,- fSize );

	pVertex[14] = vec3( fSize,- fSize,- fSize );

	pVertex[15] = vec3( fSize,- fSize, fSize );

	//face 5
	pVertex[16] = vec3( fSize,- fSize,- fSize );

	pVertex[17] = vec3(- fSize,- fSize,- fSize );

	pVertex[18] = vec3(- fSize,- fSize, fSize );

	pVertex[19] = vec3( fSize,- fSize, fSize );

	//face 6
	pVertex[20] = vec3( fSize, fSize, fSize );

	pVertex[21] = vec3(- fSize, fSize, fSize );

	pVertex[22] = vec3(- fSize, fSize,- fSize );

	pVertex[23] = vec3( fSize, fSize,- fSize );
	
	glBindBuffer(GL_ARRAY_BUFFER, m_iBufferID);
	
	glBufferData( GL_ARRAY_BUFFER, iSizeVec3, pVertex, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	delete [] pVertex;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
/*
void SkyBox::BuildSphere( float fSize, unsigned int iSlices, unsigned int iStacks )
{
	glGenBuffers(1, &m_iBufferID2 );

	unsigned int iVertexCount = m_iVertexCount = (iSlices * 4 ) * iStacks / 2;

	unsigned int iSizeVertex = iVertexCount * sizeof(vec3);
	vec3 * pVertex = new vec3[ iVertexCount ];

	float fAngleY = 2.0 * M_PI / iSlices;
	float fAngleX = 2.0 * M_PI / iStacks;

	for( unsigned int i = 0; i < iStacks/2; ++i )
	{
		for(unsigned int j = 0; j < iSlices; ++j )
		{
			float fAngleY1 = j * fAngleY;
			float fAngleY2 = fAngleY1 + fAngleY;

			float fAngleX1 = -M_PI * 0.5f + i * fAngleX;
			float fAngleX2 = fAngleX1 + fAngleX;
			
			int iOffset = i*(iSlices*4);
			
			pVertex[j*4+iOffset] = vec3( cosf(fAngleX1)*cosf(fAngleY1), sinf(fAngleX1), cosf(fAngleX1)*sinf(fAngleY1) ) * fSize;
			pVertex[j*4+1+iOffset] = vec3( cosf(fAngleX2)*cosf(fAngleY1), sinf(fAngleX2), cosf(fAngleX2)*sinf(fAngleY1) ) * fSize;
			pVertex[j*4+2+iOffset] = vec3( cosf(fAngleX1)*cosf(fAngleY2), sinf(fAngleX1), cosf(fAngleX1)*sinf(fAngleY2) ) * fSize;
			pVertex[j*4+3+iOffset] = vec3( cosf(fAngleX2)*cosf(fAngleY2), sinf(fAngleX2), cosf(fAngleX2)*sinf(fAngleY2) ) * fSize;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_iBufferID2 );
	
	glBufferData( GL_ARRAY_BUFFER, iSizeVertex, pVertex, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	delete [] pVertex;
}
*/
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SkyBox::Draw( const vec3& f3Position, unsigned int iFlag ) const
{
	glPushMatrix();
	glTranslatef( f3Position.x, f3Position.y, f3Position.z );

	if( m_pMaterial->Activate( EffectTechnique::E_RENDER_OPAQUE ) )
	{
		//if( iFlag )
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_iBufferID );
	
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUADS, 0, I_VERTEX_COUNT );
			glDisableClientState(GL_VERTEX_ARRAY); 
	
			glBindBuffer(GL_ARRAY_BUFFER, 0 );
		}
		/*
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_iBufferID2 );
	
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays( GL_TRIANGLE_STRIP, 0, m_iVertexCount );
			glDisableClientState(GL_VERTEX_ARRAY); 
	
			glBindBuffer(GL_ARRAY_BUFFER, 0 );
		}
		*/
		m_pMaterial->Deactivate( EffectTechnique::E_RENDER_OPAQUE );
	}
	glPopMatrix();
}






