/*************************************
*
*		BurgerEngine Project
*		
*		Created :	11/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#include <iostream>
#include "BurgerEngine/Core/StageManager.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Input/SFMLInputManager.h"

#include "BurgerEngineDemo/Stage/MainStage.h"

#include "BurgerEngineDemo/Core/DemoObjectFactory.h"
#include "BurgerEngineDemo/Manager/GameplayManager.h"

int main(int argc, char **argv)
{
	Engine& rEngine = Engine::GrabInstance();
	rEngine.Init( new DemoObjectFactory() );

	MainStage* pStage = new MainStage("BurgerEngine");
	pStage->Init();

	rEngine.GetStageManager().AddStage(pStage);
	rEngine.GetStageManager().SetCurrentStage("BurgerEngine");

	rEngine.Run();

	//if we exit the run loop it's over
	GameplayManager::KillInstance();
	Engine::KillInstance();
	return 0 ;
}

