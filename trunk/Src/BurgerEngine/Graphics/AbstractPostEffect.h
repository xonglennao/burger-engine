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

#ifndef __ABSTRACTPOSTEFFECT_H__
#define __ABSTRACTPOSTEFFECT_H__

#include <string>

class AbstractPostEffect
{
public:
	
	AbstractPostEffect(std::string const& sId);
	virtual ~AbstractPostEffect() {};

	virtual void PreProcess(unsigned int windowWidth, unsigned int windowHeight) = 0;
	virtual void Process(unsigned int windowWidth, unsigned int windowHeight) = 0;

	inline std::string const& GetId(){return m_sId;}

private:
	std::string		m_sId;

};


#endif //__ABSTRACTPostEFFECT_H__