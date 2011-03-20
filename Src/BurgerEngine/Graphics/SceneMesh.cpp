#include "BurgerEngine/Graphics/SceneMesh.h"

#include "BurgerEngine/Graphics/StaticMesh.h"
#include "BurgerEngine/Graphics/Material.h"

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
SceneMesh::SceneMesh( StaticMesh * pMesh )
	: m_pMesh( pMesh )
	, m_uPartCount( 0 )
	, m_bCastShadow( true )
{
	m_pBoundingBox = new float[6];
}

SceneMesh::~SceneMesh()
{
	delete [] m_pBoundingBox;
	m_pBoundingBox = NULL;
}
//--------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------
void SceneMesh::Draw( EffectTechnique::RenderingTechnique eTechnique )
{
	ComputeBoundingBox();
	if( m_pMesh )
	{
		glPushMatrix();
		glTranslatef( m_f3Position.x, m_f3Position.y, m_f3Position.z );
		
		vec3 C = vec3( m_mRotationMatrix.rows[0].z, m_mRotationMatrix.rows[1].z, m_mRotationMatrix.rows[2].z );//(mRotation * vec4(0.0f,0.0f,1.0f,1.0f)).xyz();
		vec3 M = vec3( -m_mRotationMatrix.rows[0].y, -m_mRotationMatrix.rows[1].y, -m_mRotationMatrix.rows[2].y );//(mRotation * vec4(0.0f,-1.0f,0.0f,1.0f)).xyz();
		vec3 Q = cross(C,M);
		float OR[16] = { Q[0], Q[1], Q[2], 0.0f, -M[0], -M[1], -M[2],0.0f, C[0], C[1], C[2],0.0f,0.0f,0.0f,0.0f,1.0f };
		glMultMatrixf( OR );
		
		glScalef( m_fScale, m_fScale, m_fScale);

		for( unsigned int i = 0; i < m_uPartCount; ++i )
		{
			if( eTechnique == EffectTechnique::E_RENDER_SHADOW_MAP )
			{
				m_pMesh->Render( i );
			}
			else if( m_vMaterials[ i ]->Activate( eTechnique ) )
			{
				m_pMesh->Render( i );
				m_vMaterials[ i ]->Deactivate( eTechnique );
			}
		}
		glPopMatrix();
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool SceneMesh::IsOpaque()
{
	std::vector<Material*>::iterator oIt = 	m_vMaterials.begin();
	while ( oIt != m_vMaterials.end() )
	{
		if( !(*oIt)->IsOpaque() )
			return false;
		++oIt;
	}	
	return true;
}

bool SceneMesh::IsTransparent()
{
	std::vector<Material*>::iterator oIt = 	m_vMaterials.begin();
	while ( oIt != m_vMaterials.end() )
	{
		if( (*oIt)->IsTransparent() )
			return true;
		++oIt;
	}	
	return false;
}

void SceneMesh::ComputeBoundingBox()
{
	const std::vector<vec3>& vVertex = m_pMesh->GetVertex();

	vec3 f3Point = vVertex[0] * m_fScale;
	f3Point = ( m_mRotationMatrix * vec4( f3Point, 1.0f ) ).xyz();
	f3Point = f3Point + m_f3Position;
				
	m_pBoundingBox[0] = m_pBoundingBox[1] = f3Point.x;
	m_pBoundingBox[2] = m_pBoundingBox[3] = f3Point.y;
	m_pBoundingBox[4] = m_pBoundingBox[5] = f3Point.z;
	
	//glPointSize(15.0f);
	//glBegin(GL_POINTS);
	//glVertex3f( f3Point.x, f3Point.y, f3Point.z );

	for( unsigned int i = 1; i < vVertex.size(); ++i )
	{
		f3Point = vVertex[i] * m_fScale;
		f3Point = ( m_mRotationMatrix * vec4( f3Point, 1.0f ) ).xyz();
		f3Point = f3Point + m_f3Position;
				
		m_pBoundingBox[0] = min( m_pBoundingBox[0], f3Point.x );
		m_pBoundingBox[1] = max( m_pBoundingBox[1], f3Point.x );

		m_pBoundingBox[2] = min( m_pBoundingBox[2], f3Point.y );
		m_pBoundingBox[3] = max( m_pBoundingBox[3], f3Point.y );

		m_pBoundingBox[4] = min( m_pBoundingBox[4], f3Point.z );
		m_pBoundingBox[5] = max( m_pBoundingBox[5], f3Point.z );

		//glVertex3f( f3Point.x, f3Point.y, f3Point.z );
	}
	//glEnd();
}




