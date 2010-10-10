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

/// \name	SceneMesh.h
/// \brief	The scene object which has a mesh

class SceneMesh: public SceneObject
{
public:

	/// \brief Constructor
	/// \param[in] pMesh Pointer to the actual mesh?
	SceneMesh( StaticMesh* pMesh );

	~SceneMesh(){}

public: 
	void Draw( EffectTechnique::RenderingTechnique eTechnique );

	void SetScale( float fValue ){ m_fScale = fValue; }
	void SetPartCount( unsigned int iValue ){ m_uPartCount = iValue; }

	float const	GetScale() const { return m_fScale; }
	unsigned int const GetPartCount( int iValue ) const { return m_uPartCount; }

	void AddMaterial( Material * pMaterial ){ m_vMaterials.push_back( pMaterial ); }

private:	
	std::vector<Material*>	m_vMaterials;
	unsigned int m_uPartCount;
	StaticMesh*	m_pMesh;
	float	m_fScale;
};

#endif //__SCENEMESH_H__