/*************************************
*
*		BurgerEngine Project
*		
*		Created :	26/06/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __STAGEMANAGER_H__
#define __STAGEMANAGER_H__

///	\name StageManager.h
///	\brief Manager all our stages

#include <string>
#include <map>

class AbstractStage;
class AbstractCamera;

class StageManager
{

public:
	/// \brief Constructor/Desctructor
	StageManager();
	virtual ~StageManager(){Terminate();}

	/// \brief Add a Stage to the manager
	void AddStage(AbstractStage* pStage);

	/// \brief Set the current stage running
	void SetCurrentStage(const std::string& sName);

	/// \brief Erase every data in the manager
	void Terminate();

	/// \brief Update the current stage
	void Update();

	/// \brief Reset the current Stage
	void Reset();

	/// \brief Check if the current stage is done.
	bool IsFinished();

	/// \brief Get the next stage from the current stage
	const std::string& GetNextStageId();
	
private:

	/// \brief Get the current stage for internal purpose
	AbstractStage& _GrabCurrentStage();



private:

	/// Map which containt all scene
	std::map<std::string,AbstractStage*> m_mStages;

	/// Pointer to the current scene.
	AbstractStage* m_pCurrentStage;
};

#endif //__STAGEMANAGER_H__
