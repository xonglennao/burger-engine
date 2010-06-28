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

#ifndef __ABSTRACTSTAGE_H__
#define __ABSTRACTSTAGE_H__


///	\name AbstractStage.h
///	\brief Virtual Class of a stage
///	\date 18/12/09


#include <string>


class AbstractStage
{
public:
	AbstractStage(std::string const& a_sId);
	virtual ~AbstractStage();

	/// \brief	initialize a scene
	virtual bool Init() = 0;

	/// \brief	Main dipslay method
	void Display();

	/// \brief Update the scene
	virtual void Update(); 

	/// \brief Return true is the stage is over.
	virtual bool IsFinished();

	/// \brief ReInit the stage;
	virtual void Reset();

	/// \brief Getter
	inline const std::string& getSuccessorId(){return m_sNextStageId;}
	inline const std::string& getName(){return m_sId;}

protected:
	/// \brief	Pre-display method can set FBO and such.
	virtual void _PreDisplay();

	/// \brief	Internal daiplay method for child class.
	virtual void _Display() = 0;

	/// \brief	Post-render method can free FBO, release stuff...
	virtual void _PostDisplay();
	

private:

	std::string m_sId;
	std::string m_sNextStageId;

};


#endif //__ABSTRACTSTAGE_H__


