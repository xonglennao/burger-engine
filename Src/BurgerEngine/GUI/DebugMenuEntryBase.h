/*************************************
*
*		BurgerEngine Project
*		
*		Created :	14/02/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __DEBUGMENUENTRYBASE_H__
#define __DEBUGMENUENTRYBASE_H__

/// \class DebugMenuBase
/// \brief A string/numeric value pair
#include <string>
#include <sstream>
class DebugMenuEntryBase
{

public:

	/// \brief default constructor
	DebugMenuEntryBase(){}

	/// \brief Destructor
	virtual ~DebugMenuEntryBase(){}

	virtual void Increment(){};
	virtual void Decrement(){};
	virtual std::string GetValue(){ std::stringstream oStream; return oStream.str(); };

protected:
private:
};

#endif //__DEBUGMENUENTRYNBASE_H__
