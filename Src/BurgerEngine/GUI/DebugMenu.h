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

#ifndef __DEBUGMENU_H__
#define __DEBUGMENU_H__

#include <string>
#include <vector>
#include <utility>

#include "BurgerEngine/GUI/DebugMenuEntry.h"

/// \class DebugMenu
/// \brief DebugMenu that can be used to changed the values of some variables in real-time
class DebugMenu
{
private:
public:
	enum DebugMenuEnum
	{
		 E_INCREMENT		=	1
		,E_DECREMENT		=	1 << 1 
		,E_NEXT				=	1 << 2
		,E_PREVIOUS			=	1 << 3
	};
	/// \brief default constructor
	DebugMenu();

	/// \brief Destructor
	~DebugMenu();
	
	template <class T>
	void AddEntry( const std::string& sName, T & tValue, T tMin, T tMax, T tStep )
	{
		DebugMenuEntryBase * pEntry= new DebugMenuEntry<T>( tValue, tMin, tMax, tStep );
		m_oDebugMenu.push_back( std::pair<std::string, DebugMenuEntryBase* >( sName, pEntry ) );
	}

	void	Update( float fDeltaTime );
	void	SetFlag( DebugMenuEnum eFlag, bool bValue );

	void	Display();
	const	std::pair< std::string, DebugMenuEntryBase* >& GetEntry() const { return m_oDebugMenu[ m_oCurrentPosition ]; };
protected:

private:
	void Increment();
	void Decrement();
	void Next();
	void Previous();

	std::vector< std::pair< std::string, DebugMenuEntryBase* > > m_oDebugMenu;
	
	unsigned int	m_oCurrentPosition;
	
	int				m_iFlags;
	float			m_fTimeSinceLastUpdate;;
};

#endif //__DEBUGMENU_H__
