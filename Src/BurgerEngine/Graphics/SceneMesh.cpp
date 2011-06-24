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
	, m_fScale( 1.0f )
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
	//glDisable( GL_DEPTH_TEST );

	const float * pBoundingBox = m_pMesh->GetBoundingBox();

	std::vector<vec3> vf3Point3;
	for( unsigned int i = 0; i < 2; ++i )
	{
		for( unsigned int j = 2; j < 4; ++j )
		{
			for( unsigned int k = 4; k < 6; ++k )
			{
				vf3Point3.push_back( vec3( pBoundingBox[i], pBoundingBox[j], pBoundingBox[k] ) );
			}
		}
	}

	vf3Point3[0] = vf3Point3[0] * m_fScale;
	vf3Point3[0] = ( m_mRotationMatrix * vec4( vf3Point3[0], 1.0f ) ).xyz();
	vf3Point3[0] = vf3Point3[0] + m_f3Position;
				
	m_pBoundingBox[0] = m_pBoundingBox[1] = vf3Point3[0].x;
	m_pBoundingBox[2] = m_pBoundingBox[3] = vf3Point3[0].y;
	m_pBoundingBox[4] = m_pBoundingBox[5] = vf3Point3[0].z;
	
	//glPointSize(3.0f);
	//glBegin(GL_POINTS);
	//glVertex3f( vf3Point3[0].x, vf3Point3[0].y, vf3Point3[0].z );

	std::vector<vec3>::iterator oIt = vf3Point3.begin();
	std::vector<vec3>::iterator oEnd = vf3Point3.end();

	++oIt;

	while( oIt != oEnd )
	{
		vec3 f3Point = (*oIt) * m_fScale;
		f3Point = ( m_mRotationMatrix * vec4( f3Point, 1.0f ) ).xyz();
		f3Point = f3Point + m_f3Position;
				
		m_pBoundingBox[0] = min( m_pBoundingBox[0], f3Point.x );
		m_pBoundingBox[1] = max( m_pBoundingBox[1], f3Point.x );

		m_pBoundingBox[2] = min( m_pBoundingBox[2], f3Point.y );
		m_pBoundingBox[3] = max( m_pBoundingBox[3], f3Point.y );

		m_pBoundingBox[4] = min( m_pBoundingBox[4], f3Point.z );
		m_pBoundingBox[5] = max( m_pBoundingBox[5], f3Point.z );

		//glVertex3f( f3Point.x, f3Point.y, f3Point.z );
		++oIt;
	}

	//glEnd();
	//glEnable( GL_DEPTH_TEST );
}




