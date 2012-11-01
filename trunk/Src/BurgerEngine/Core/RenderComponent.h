/*************************************
*
*		BurgerEngine Project
*		
*		Created :	16/02/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __RENDERCOMPONENT_H__
#define __RENDERCOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"

class SceneMesh;

class RenderComponent : public AbstractComponent
{
public:
	/// \brief constructor
	RenderComponent(CompositeComponent* a_pParent = NULL);

	/// \brief Copy constructor //clone???
	RenderComponent(AbstractComponent const& a_rToCopy);

	/// \brief Destructor
	~RenderComponent();

	/// \brief Load the component using a XML Node
	/// override
	void Initialize(TiXmlElement const& a_rParameters);

	/// \brief Grab the mesh
	/// \todo Could also go from the RenderContext
	SceneMesh& GrabInternalMesh(){assert(m_pMesh); return *m_pMesh;}

	void Update( float fFrameTime, float fElapsedTime );

private:
	/// Own a Scene mesh object
	SceneMesh* m_pMesh;
};

#endif //__RENDERCOMPONENT_H__