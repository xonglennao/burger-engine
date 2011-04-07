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
#ifndef __COMPOSITECOMPONENT_H__
#define __COMPOSITECOMPONENT_H__

#include <vector>

#include "BurgerEngine/Core/AbstractComponent.h"

class CompositeComponent : public AbstractComponent
{
public:
	/// \brief constructor
	CompositeComponent(CompositeComponent* a_pParent);

	/// \brief Copy constructor //clone???
	CompositeComponent(AbstractComponent const& a_rToCopy);

	/// \brief Destructor
	~CompositeComponent();

	/// \brief Load the component using a XML Node
	/// override
	void Initialize(TiXmlElement const& a_rParameters);

	/// \brief Get a reference to the component
	//template<typename t_CptType>
	//t_CptType const& GetComponentByType() const;
	// \todo to be uncomment when invalid ref are possible
	//AbstractComponent const& GetComponentByType(ComponentType a_eType) const;

	/// \brief Grab a reference to the component
	//AbstractComponent& GrabComponentByType(ComponentType a_eType);

	/// \brief Try to get a component, if not working then return a pointer to null
	AbstractComponent const* TryGetComponentByType(ComponentType a_eType) const;

	/// \brief Try to grab a component, if not working then return a pointer to null
	AbstractComponent* TryGrabComponentByType(ComponentType a_eType);

	/// \brief Grab all the component of the type, with the same level (will not go into other leaf)
	/// \todo to template
	std::vector<AbstractComponent*> TryGrabComponentVectorByType( ComponentType a_eType );


	/// \brief [OVERIDE] Set the position, and right now alert the render component
	/// \todo we need to find a way to alert every component needed, but it might be
	/// heavy with listener
	void SetPos( vec3 const& a_vValue );

	/// \brief Update all render componenent with a new scale
	/// \todo again this is kinda specific so it's pretty ugly, should be change
	void SetScale( float const a_fValue );

protected:
	/// \brief Inner update
	void _Update();

private:
	/// \brief Collection Of AbstractComponenet
	std::vector<AbstractComponent*> m_vComponents;
};

#endif //__COMPOSITECOMPONENT_H_