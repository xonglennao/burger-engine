/*************************************
*
*		BurgerEngine Project
*		
*		Created :	25/06/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

///	\name singleton.h
///	\brief Singleton template class

#include "CommonBase.h"

template <typename T> class Singleton //typename instead of class, since it can recieve anything
{
protected:
	static T* s_pInstance; 
		
	/// \brief Constructor - Destructor
		
	Singleton(){}
	virtual ~Singleton(){}

		
	/// TODO : Copy Constructor - "=" operator

public:

	/// \brief Only way to get the singleton
	static inline T& GrabInstance()
	{
		if (s_pInstance == NULL)
		{
			s_pInstance = new T;
		}
		return *s_pInstance;
	}

	static inline T const& GetInstance()
	{
		assert(s_pInstance != NULL);
		return *s_pInstance;
	}

	/// \brief Only way to kill the singleton
	static inline void KillInstance()
	{
		if (s_pInstance != NULL)
			delete s_pInstance;
		s_pInstance = NULL;
	}

};

//	Set our Singleton to Zero

template<typename T> T* Singleton<T>::s_pInstance = NULL;

/*
	In order to create a singleton, need this :
	class OnlyOne : public Singleton<OnlyOne>
 {
     // constructeurs/destructeur de OnlyOne accessibles au Singleton
     friend class Singleton<OnlyOne>; 
 };
 {from Wikipedia}
*/


#endif //__SINGLETON_H__
