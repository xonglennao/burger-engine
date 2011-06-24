/*************************************
*
*		BurgerEngine Project
*		
*		Created :	15/04/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/
#ifndef __LIGHTCOMPONENT_H__
#define __LIGHTCOMPONENT_H__

#include "BurgerEngine/Core/AbstractComponent.h"
#include "BurgerEngine/Graphics/SceneLight.h"
#include <map>
#include <string>



class LightComponent : public AbstractComponent
{
public:
	/// \brief constructor
	LightComponent(CompositeComponent* a_pParent = NULL);

	/// \brief Copy constructor //clone???
	LightComponent(AbstractComponent const& a_rToCopy);

	/// \brief Destructor
	~LightComponent();

	/// \brief Load the component using a XML Node
	/// override
	void Initialize(TiXmlElement const& a_rParameters);

	/// \brief Grab the light
	/// \todo Could also go from the RenderContext
	SceneLight& GrabInternalLight(){assert(m_pLight); return *m_pLight;}

	/// \brief Update the position of the light
	/// usually called when the parent is moved
	void UpdatePos();

	/// \brief Update the rotation
	void UpdateRotation();

protected:
	/// \brief Inner update
	void _Update( float fFrameTime, float fElapsedTime );

private:
	/// Own a Scene light object
	SceneLight* m_pLight;

	/// Loading type array
	/// \todo this should be a static array (no point to being a member)
	/// but I can't make it work
	std::map< std::string, SceneLight::LightType > m_mStringToLightTypeMap;
};



#endif //__LIGHTCOMPONENT_H__