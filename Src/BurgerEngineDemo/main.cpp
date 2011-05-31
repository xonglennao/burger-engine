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

#include "BurgerEngine/Core/StageMain3DScene.h"

int main(int argc, char **argv)
{
	char* pSceneName = NULL;
	for( int i = 1; i < argc; ++i )
	{
		std::string oString = argv[i];
		if( oString == "-scene" )
		{
			++i;
			if( i < argc )
			{
				pSceneName = argv[i];
			}
		}
	}

	assert( pSceneName );

	Engine& rEngine = Engine::GrabInstance();
	rEngine.Init( pSceneName );

	//TEMP franck: Test Input
	//StageTestInput* pStage = new StageTestInput("TestInput");
	StageMain3DScene* pStage = new StageMain3DScene("TestInput");
	pStage->Init();

	rEngine.GetStageManager().AddStage(pStage);
	rEngine.GetStageManager().SetCurrentStage("TestInput");

	rEngine.Run();

	//if we exit the run loop it's over
	Engine::KillInstance();
	return 0 ;
}
