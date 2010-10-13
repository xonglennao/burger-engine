#include "BurgerEngine/Graphics/SpotLight.h"

#include "BurgerEngine/External/Math/Vector.h"

SpotLight::SpotLight()
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::ComputeBoundingVolume()
{
	float4x4 mRotation = rotateY( m_f3Rotation.y * (float)M_PI / 180.0f ) * rotateX( m_f3Rotation.x * (float)M_PI / 180.0f );
	vec4 vWorldSpaceRight = mRotation * vec4( 1.0f, 0.0f, 0.0f, 0.0f );
	vec4 vWorldSpaceUp = mRotation * vec4( 0.0f, 1.0f, 0.0f, 0.0f );
	vec4 vWorldSpaceDir = mRotation * vec4( 0.0f, 0.0f, -1.0f, 0.0f );

	float fOpp = tanf( acosf( m_fCosOut ) ) * m_fRadius;

	vec3 f3RadiusVector = m_fRadius * vec3( vWorldSpaceDir.x, vWorldSpaceDir.y, vWorldSpaceDir.z );
	vec3 f3Right = fOpp * vec3( vWorldSpaceRight.x,vWorldSpaceRight.y,vWorldSpaceRight.z);
	vec3 f3Up = fOpp * vec3( vWorldSpaceUp.x,vWorldSpaceUp.y,vWorldSpaceUp.z);

	vec3 pNearPlanePoints[4];
	pNearPlanePoints[0] = m_f3Position + f3Right + f3Up;
	pNearPlanePoints[1] = m_f3Position - f3Right + f3Up;
	pNearPlanePoints[2] = m_f3Position + f3Right - f3Up;
	pNearPlanePoints[3] = m_f3Position - f3Right - f3Up;

	m_pFarPlanePoints[0] = f3RadiusVector + pNearPlanePoints[0];
	m_pFarPlanePoints[1] = f3RadiusVector + pNearPlanePoints[1];
	m_pFarPlanePoints[2] = f3RadiusVector + pNearPlanePoints[2];
	m_pFarPlanePoints[3] = f3RadiusVector + pNearPlanePoints[3];
	
	float fXmin, fXmax, fYmin, fYmax, fZmin, fZmax;
	fXmin = fXmax = m_pFarPlanePoints[0].x;
	fYmin = fYmax = m_pFarPlanePoints[0].y;
	fZmin = fZmax = m_pFarPlanePoints[0].z;

	for( unsigned int i = 1; i < 4; ++i )
	{
		fXmin = min(fXmin, m_pFarPlanePoints[i].x);
		fXmax = max(fXmax, m_pFarPlanePoints[i].x);

		fYmin = min(fYmin, m_pFarPlanePoints[i].y);
		fYmax = max(fYmax, m_pFarPlanePoints[i].y);

		fZmin = min(fZmin, m_pFarPlanePoints[i].z);
		fZmax = max(fZmax, m_pFarPlanePoints[i].z);
	}

	for( unsigned int i = 0; i < 4; ++i )
	{
		fXmin = min(fXmin, pNearPlanePoints[i].x);
		fXmax = max(fXmax, pNearPlanePoints[i].x);

		fYmin = min(fYmin, pNearPlanePoints[i].y);
		fYmax = max(fYmax, pNearPlanePoints[i].y);

		fZmin = min(fZmin, pNearPlanePoints[i].z);
		fZmax = max(fZmax, pNearPlanePoints[i].z);
	}

	m_pBoundingBox[0] = fXmin;
	m_pBoundingBox[1] = fXmax;

	m_pBoundingBox[2] = fYmin;
	m_pBoundingBox[3] = fYmax;

	m_pBoundingBox[4] = fZmin;
	m_pBoundingBox[5] = fZmax;
}