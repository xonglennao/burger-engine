/*************************************
*
*		BurgerEngine Project
*		
*		Created :	21/08/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __SCENEMESH_H__
#define __SCENEMESH_H__

#include <vector>
#include "BurgerEngine/Graphics/SceneObject.h"
#include "BurgerEngine/Graphics/EffectTechnique.h"

class StaticMesh;
class Material;

class SceneMesh
	: public SceneObject
{
public:
	SceneMesh( StaticMesh * pMesh );
	~SceneMesh();
	void Draw( EffectTechnique::RenderingTechnique eTechnique );

public: 
	inline void			SetScale( float fValue ){ fScale = fValue; };
	inline void			SetPartCount( int iValue ){ m_iPartCount = iValue; };
	inline const float	GetScale() const { return fScale; };
	inline const int	GetPartCount( int iValue ) const { return m_iPartCount; };
	inline void			AddMaterial( Material * pMaterial ){ m_oMaterials.push_back( pMaterial ); }

private:	
	std::vector< Material * >	m_oMaterials;
	unsigned int				m_iPartCount;
	StaticMesh*					m_pMesh;

	float	fScale;
};

#endif //__SCENEMESH_H__