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

class AbstractComponent
{
public:
	/// \brief constructor
	AbstractComponent(){}

	/// \brief Copy constructor
	AbstractComponent(AbstractComponent const& a_rToCopy){}

	/// \brief Destructor
	virtual ~AbstractComponent(){}

	/// \brief Load the component using a XML Node
	/// \todo Shoudl not be an xml node (maybe a InitParam or something)
	virtual void Initialize(TiXmlElement const& a_rParameters) = 0;

	///\brief Update
	void Update();

protected:
	/// \brief Inner update
	virtual void _Update() = 0;

};

#endif //__ABSTRACTCOMPONENT_H__