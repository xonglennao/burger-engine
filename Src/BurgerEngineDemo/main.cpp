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

/// \TEMP Franck: Input test
#include "StageTestInput.h"

int main(int argc, char **argv)
{
	Engine& rEngine = Engine::GrabInstance();

	rEngine.Init();

	//TEMP franck: Test Input
	StageTestInput* pStage = new StageTestInput("TestInput");
	pStage->Init();

	rEngine.GetStageManager().AddStage(pStage);
	rEngine.GetStageManager().SetCurrentStage("TestInput");

	rEngine.Run();

	//if we exit the run loop it's over
	Engine::KillInstance();
	return 0 ;
}

