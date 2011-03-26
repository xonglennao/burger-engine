#include "BurgerEngine/Graphics/SpotLight.h"

#include "BurgerEngine/External/Math/Vector.h"

SpotLight::SpotLight()
{
	m_pBoundingBox = new float[6];
}

SpotLight::~SpotLight()
{
	delete [] m_pBoundingBox;
	m_pBoundingBox = NULL;
}

void SpotLight::ComputeBoundingBox()
{
	//float4x4 mRotation = rotateY( m_f3Rotation.y * DEG_TO_RAD ) * rotateX( m_f3Rotation.x * DEG_TO_RAD );
	
	float fOpp = tanf( acosf( m_fCosOut ) ) * m_fRadius;

	vec3 f3Right = fOpp * vec3( m_mRotationMatrix.rows[0].x, m_mRotationMatrix.rows[1].x, m_mRotationMatrix.rows[2].x );
	vec3 f3Up = fOpp * vec3( m_mRotationMatrix.rows[0].y, m_mRotationMatrix.rows[1].y, m_mRotationMatrix.rows[2].y );

	vec3 f3FarPos = -m_fRadius * vec3( m_mRotationMatrix.rows[0].z, m_mRotationMatrix.rows[1].z, m_mRotationMatrix.rows[2].z ) + m_f3Position;

	m_pFarPlanePoints[0] = f3FarPos + f3Right + f3Up;
	m_pFarPlanePoints[1] = f3FarPos - f3Right + f3Up;
	m_pFarPlanePoints[2] = f3FarPos + f3Right - f3Up;
	m_pFarPlanePoints[3] = f3FarPos - f3Right - f3Up;
	
	m_pBoundingBox[0] = m_pBoundingBox[1] = m_f3Position.x;
	m_pBoundingBox[2] = m_pBoundingBox[3] = m_f3Position.y;
	m_pBoundingBox[4] = m_pBoundingBox[5] = m_f3Position.z;

	for( unsigned int i = 0; i < 4; ++i )
	{
		m_pBoundingBox[0] = min(m_pBoundingBox[0], m_pFarPlanePoints[i].x);
		m_pBoundingBox[1] = max(m_pBoundingBox[1], m_pFarPlanePoints[i].x);

		m_pBoundingBox[2] = min(m_pBoundingBox[2], m_pFarPlanePoints[i].y);
		m_pBoundingBox[3] = max(m_pBoundingBox[3], m_pFarPlanePoints[i].y);

		m_pBoundingBox[4] = min(m_pBoundingBox[4], m_pFarPlanePoints[i].z);
		m_pBoundingBox[5] = max(m_pBoundingBox[5], m_pFarPlanePoints[i].z);
	}
}