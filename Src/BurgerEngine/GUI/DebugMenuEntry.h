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

#ifndef __DEBUGMENUENTRY_H__
#define __DEBUGMENUENTRY_H__

#include "BurgerEngine/GUI/DebugMenuEntryBase.h"
#include <string>
#include <sstream>
/// \class AbstractMenuDebug
/// \brief A templated string/numeric value pair
template <class T>
class DebugMenuEntry : public DebugMenuEntryBase
{

public:

	/// \brief default constructor
	DebugMenuEntry( T & tValue, T tMin, T tMax, T tStep )
	:	  m_pValue ( &tValue )
		, m_tMin ( tMin )
		, m_tMax ( tMax )
		, m_tStep ( tStep )
	{
	}

	/// \brief Destructor
	virtual ~DebugMenuEntry(){}

	
	void Increment()
	{
		(*m_pValue) = ( (*m_pValue) += m_tStep ) > m_tMax ? m_tMax : (*m_pValue);
	}

	void Decrement()
	{
		(*m_pValue) = ( (*m_pValue) -= m_tStep ) < m_tMin ? m_tMin : (*m_pValue);
	}

	std::string GetValue()
	{
		std::stringstream oStream;
		oStream << (*m_pValue);
		return oStream.str();
	}


protected:
private:
	T * m_pValue;
	T m_tMin;
	T m_tMax;
	T m_tStep;
};

#endif //__DEBUGMENUENTRY_H__
