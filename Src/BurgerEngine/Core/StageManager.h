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

#include "BurgerEngine/Base/Singleton.h"


class AbstractStage;
class AbstractCamera;

class StageManager:public Singleton<StageManager>
{
	friend class Singleton<StageManager>;

public:

	/// \brief Add a Stage to the manager
	void AddStage(AbstractStage* pScene);

	/// \brief Set the current stage running
	void SetCurrentStage(const std::string& sName);

	/// \brief Erase every data in the manager
	void Clear();

	/// \brief Update the current stage
	void Update();

	/// \brief Display the current stage
	void Display();
	
	/// \brief Reset the current Stage
	void Reset();

	/// \brief Check if the current stage is done.
	bool IsFinished();

	/// \brief Get the next stage from the current stage
	const std::string& GetNextStageId();
	
private:

	/// \brief Get the current stage for internal purpose
	AbstractStage& _GrabCurrentStage();

	/// \brief Constructor/Desctructor
	StageManager();
	virtual ~StageManager(){Clear();};

private:

	/// Map which containt all scene
	std::map<std::string,AbstractStage*> m_mStages;

	/// Pointer to the current scene.
	AbstractStage* m_pCurrentStage;
};

#endif //__STAGEMANAGER_H__
