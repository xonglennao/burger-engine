/*************************************
*
*		BurgerEngine Project
*		
*		Created :	25/04/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __PARTICLECOMPONENT_H__
#define __PARTICLECOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"

class ParticleSystem;

class ParticleComponent : public AbstractComponent
{
public:
	/// \brief constructor
	ParticleComponent(CompositeComponent* a_pParent = NULL);

	/// \brief Copy constructor //clone???
	ParticleComponent(AbstractComponent const& a_rToCopy);

	/// \brief Destructor
	~ParticleComponent();

	/// \brief Load the component using a XML Node
	/// override
	void Initialize(TiXmlElement const& a_rParameters);


protected:
	/// \brief Inner update
	void _Update( float fFrameTime, float fElapsedTime );

private:

	/// The contained system
	ParticleSystem* m_pSystem;
};

#endif //__PARTICLECOMPONENT_H__