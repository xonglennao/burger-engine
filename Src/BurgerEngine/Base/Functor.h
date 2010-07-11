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



#ifndef __FUNCTOR_H__
#define __FUNCTOR_H__

#include "CommonBase.h"
#include <memory>

/// \class	FunctorBase
/// \brief	
///			

/*template <typename tResult, template <class, class> >
class FunctorBase
{

public:

	typedef tResult ResultType;

    typedef EmptyType Parm1;
    typedef EmptyType Parm2;
    typedef EmptyType Parm3;
    typedef EmptyType Parm4;
    typedef EmptyType Parm5;
    typedef EmptyType Parm6;
    typedef EmptyType Parm7;
    typedef EmptyType Parm8;
    typedef EmptyType Parm9;
    typedef EmptyType Parm10;
    typedef EmptyType Parm11;
    typedef EmptyType Parm12;
    typedef EmptyType Parm13;
    typedef EmptyType Parm14;
    typedef EmptyType Parm15;

	virtual ~FunctorBase(){}

};


template <typename tResult, class TList, template <class, class> >
class FunctorImpl;

template <typename tResult, template <class, class> >
class FunctorImpl<tResult> : public FunctorBase<tResult>
{
public:
	typedef tResult ResultType;
    virtual tResult operator()() = 0;
};

template <typename tResult, typename tParam1, template <class, class> >
class FunctorImpl<tResult> : public FunctorBase<tResult>
{
public:
	typedef tResult ResultType;
    virtual tResult operator()() = 0;
};

template <typename tResult, template <class, class> >
class FunctorImpl<tResult> : public FunctorBase<tResult>
{
public:
	typedef tResult ResultType;
    virtual tResult operator()() = 0;
};

template <typename tResult, template <class, class> >
class FunctorImpl<tResult> : public FunctorBase<tResult>
{
public:
	typedef tResult ResultType;
    virtual tResult operator()() = 0;
};*/


	/// \brief Constructor
	/// \param[in] a_trObject 
	/// \param[in] a_tpMathod 
/*	FunctorHandler(tObject const& a_trObject, tpMethod* a_tpMethod):
	  m_trObject(a_trObject),
	  m_tpMethod(a_tpMethod){}
		

	/// \brief Call the stored method
	//tResultType operator()();
	void operator()();

private:
	tObject const& m_trObject;
	tpMethod* m_tpMethod;
};*/

/// \class Functor
/// \brief Template class to allow callback function

class AbstractFunctorOneChar
{
public:

    // two possible functions to call member function. virtual cause derived
    // classes will use a pointer to an object and a pointer to a member function
    // to make the function call
    virtual void operator()(unsigned char) = 0;  // call using operator
};

template <class TClass> class FunctorOneChar : public TFunctor
{
private:
    void (TClass::*m_pMethod)(unsigned char);   // pointer to member function
    TClass* m_pObject;                  // pointer to object

public:

    // constructor - takes pointer to an object and pointer to a member and stores
    // them in two private variables
    FunctorOneChar(TClass* a_pObject, void(TClass::*a_pMethod)(unsigned char)):
		m_pObject(a_pObject),
		m_pMethod(a_pMethod)
		{}

    // override operator "()"
    void operator()(unsigned char a_cChar)
    { (*a_pObject.*a_pMethod)(a_cChar);}              // execute member function

 };

#endif //__FUNCTOR_H__


