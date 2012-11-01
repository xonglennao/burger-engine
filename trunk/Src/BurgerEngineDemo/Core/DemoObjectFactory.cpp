#include "BurgerEngineDemo/Core/DemoObjectFactory.h"
#include "BurgerEngineDemo/Component/PlayerComponent.h"
#include "BurgerEngineDemo/Component/EnnemyComponent.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void DemoObjectFactory::Terminate()
{
	ObjectFactory::Terminate();
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
AbstractComponent* DemoObjectFactory::CreateComponentFromID( std::string& a_sID, CompositeComponent* a_pParent )
{
	AbstractComponent* pComponent;
	
	if ( pComponent = ObjectFactory::CreateComponentFromID( a_sID, a_pParent ) )
	{
		return pComponent;
	}

	if(a_sID == "player")
	{
		return new PlayerComponent(a_pParent);
	}
	else if(a_sID == "ennemy")
	{
		return new EnnemyComponent(a_pParent);
	}

	return NULL;
}