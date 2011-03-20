/*************************************
*
*		BurgerEngine Project
*		
*		Created :	12/03/11
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

/// \brief Log an Error Message
/// \todo this is a very first version is should not use 
/// STD and such, just simple char*
#define ADD_ERROR_MESSAGE( msg )    \
  {                    \
  std::cerr <<"[ERROR]" <<msg<<std::endl;      \
  }


/// \brief Log an Info Message
#define ADD_LOG_MESSAGE( msg )    \
  {                    \
  std::cout <<"[LOG]" <<msg<<std::endl;      \
  }

/// \brief Use a classic for( int i=0, i< a_iMax; ++i)
/// \warning a_iMEX shoud be tested with the max of integer
/// for(int a_iVal = 0, _iMax_##a_iVal = (a_iMax); a_iVal < _iMax_##a_iVal; a_iVal++)
#define FOR_EACH_INT(a_iVal, a_iMax) \
	for(int a_iVal = 0; a_iVal < a_iVal; ++a_iVal)

/// \brief Use a classic for( int i=0, i< a_iMax; ++i)
/// \warning a_iMEX shoud be tested with the max of integer
#define FOR_EACH_UINT(a_iVal, a_iMax) \
	for(unsigned int a_iVal = 0; a_iVal < a_iVal; ++a_iVal)

/// \brief use a iterator
#define FOR_EACH_IT(a_oCollectionType, a_oCollection, it ) \
	for(a_oCollectionType::iterator it = a_oCollection.begin(); it != a_oCollection.end(); ++it)

/// \brief use a  const iterator
#define FOR_EACH_IT_CONST(a_oCollectionType, a_oCollection, it ) \
	for(a_oCollectionType::const_iterator it = a_oCollection.begin(); it != a_oCollection.end(); ++it)