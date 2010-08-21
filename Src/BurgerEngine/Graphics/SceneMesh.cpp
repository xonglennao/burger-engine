#include "BurgerEngine/Graphics/SceneMesh.h"

#include "BurgerEngine/Graphics/StaticMesh.h"
#include "BurgerEngine/Graphics/Material.h"

SceneMesh::SceneMesh( StaticMesh * pMesh )
	: m_pMesh( pMesh )
{
	m_iPartCount = 0;
}

SceneMesh::~SceneMesh()
{
	std::vector< Material* >::iterator it = m_oMaterials.begin();
	while( it != m_oMaterials.end() )
	{
		delete (*it);
		(*it) = NULL;
		++it;
	}
};

void SceneMesh::Draw()
{

	if( m_pMesh )
	{
		glPushMatrix();
		glTranslatef( vPosition.x(), vPosition.y(), vPosition.z() );
		glRotatef( vRotation.x(), 1, 0, 0 );	
		glRotatef( vRotation.y(), 0, 1, 0 );
		glRotatef( vRotation.z(), 0, 0, 1 );	

		glScalef( fScale, fScale, fScale);

		for( unsigned int i = 0; i < m_iPartCount; ++i )
		{
			m_oMaterials[ i ]->Activate();
			m_pMesh->Render( i );
			m_oMaterials[ i ]->Desactivate();
		}

		glPopMatrix();

	}

}






