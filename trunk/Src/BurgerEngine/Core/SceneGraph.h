/*************************************
*
*		BurgerEngine Project
*		
*		Created :	19/09/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/


#ifndef __SCENEGRAPH_H__
#define __SCENEGRAPH_H__

#include "BurgerEngine/Graphics/CommonGraphics.h"
#include "BurgerEngine/Graphics/SceneLight.h"

#include "BurgerEngine/GUI/DebugMenu.h"
#include "BurgerEngine/Core/ObjectFactory.h"

#include <vector>
#include <map>

/// forward declaration


/// Typedef
typedef std::vector< AbstractComponent* > ComponentCollection;

/// \class	SceneGraph
/// \brief	Used to display 3D scenes
class SceneGraph
{
public:

	/// \brief Constuctor
	SceneGraph( const char * pSceneName );
	~SceneGraph();

	bool Initialize();

	/// \brief Update all the components object in the graph
	void Update( float fFrameTime, float fElapsedTime );

private:
/// \brief Clears object and light lists
	void Clear();

/// \brief Loads objects, light etc. from a XML file
	void LoadSceneXML( const char * sName );
	
private:

	///List of all the components object
	ComponentCollection m_vComponentCollection;

	/// The Factory which can create object depending on the string griven
	ObjectFactory m_oFactory;
};


#endif //__SCENEGRAPH_H__
