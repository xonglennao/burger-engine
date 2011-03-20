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
#ifndef __ABSTRACTCOMPONENT_H__
#define __ABSTRACTCOMPONENT_H__

//Forward declaration
class TiXmlElement;
class CompositeComponent;

#include "BurgerEngine/Base/CommonBase.h"
#include "BurgerEngine/External/Math/Vector.h"

/// \brief an enum with all the possible component
/// It will be used for finding the right component
/// It's not the best, but it's better than a name
/// and we cannot use dynamic cast
enum ComponentType
{
	COMPOSITE,
	RENDER,
	UNKNOWN
};


class AbstractComponent
{
public:
	/// \brief constructor
	AbstractComponent(ComponentType a_eType, CompositeComponent* a_pParent = NULL):
	m_eType(a_eType),
	m_pParentNode(a_pParent)
	{}

	/// \brief Copy constructor
	AbstractComponent(AbstractComponent const& a_rToCopy){}

	/// \brief Destructor
	virtual ~AbstractComponent(){}

	/// \brief Clone method is used by the factory
	AbstractComponent& Clone(AbstractComponent const& a_rToCopy);

	/// \brief Load the component using a XML Node
	/// \todo Shoudl not be an xml node (maybe a InitParam or something)
	virtual void Initialize(TiXmlElement const& a_rParameters) = 0;

	///\brief Update
	void Update();

	/// \brief Set and Get position
	void SetPos( vec3 const& a_vValue ){ m_f3Position = a_vValue; }
	vec3 const& GetPos() const { return m_f3Position; }

	/// \brief Get the parent of the node
	CompositeComponent const* GetParent() const {return m_pParentNode;}

	/// \brief Get the Component Type
	ComponentType GetType() const {return m_eType;}

protected: // private ???
	/// \brief Inner update
	virtual void _Update() = 0;

	/// \brief Inner Clone
	//AbstractComponent& _Clone(AbstractComponent const& a_rToCopy) = 0;

private:
	/// The component type
	ComponentType m_eType;

	/// Each components has its own position
	/// \todo maybe it's up to the component itself to has or not a position
	/// the problem is that if only the composite hold it's runtime position
	/// how the render can be inform of the composite position
	/// Position - might be better is matrix
	vec3	m_f3Position;

	/// The parent Node, which has to be a composite composent
	/// Will be useful to retrieve info of the parent, (position)
	/// Or access content of other leaf
	CompositeComponent* m_pParentNode;


};

#endif //__ABSTRACTCOMPONENT_H__