#include "BurgerEngineDemo/Manager/GameplayManager.h"
#include "BurgerEngine/External/Math/Vector.h"


//TMP, we need to have scene dependant values
const float SCENE_SIZE = 130.0f; //it's a cube

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
GameplayManager::GameplayManager() :
	m_pCurrentPlayer( NULL )
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void GameplayManager::GetRespawnPosition(vec3& f3NewPosition)
{
	int iRandom = rand() % 4;

	f3NewPosition.y = 0.0f;

	switch(iRandom)
	{
		case 0 :
			f3NewPosition.x = SCENE_SIZE;
			f3NewPosition.z = 0.0f;
		break;

		case 1 :
			f3NewPosition.x = -SCENE_SIZE;
			f3NewPosition.z = 0.0f;
		break;

		case 2 :
			f3NewPosition.x = 0.0f;
			f3NewPosition.z = SCENE_SIZE;
		break;

		case 3 :
			f3NewPosition.x = 0.0f;
			f3NewPosition.z = -SCENE_SIZE;
		break;
	}
}