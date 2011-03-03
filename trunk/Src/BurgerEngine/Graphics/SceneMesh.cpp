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
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SceneMesh::Draw( EffectTechnique::RenderingTechnique eTechnique )
{
	if( m_pMesh )
	{
		glPushMatrix();
		glTranslatef( m_f3Position.x, m_f3Position.y, m_f3Position.z );
		glRotatef( m_f3Rotation.x, 1, 0, 0 );	
		glRotatef( m_f3Rotation.y, 0, 1, 0 );
		glRotatef( m_f3Rotation.z, 0, 0, 1 );	

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





