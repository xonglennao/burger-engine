#include "DebugMenu.h"

const float TIME_BEFORE_UPDATE = 100.0;
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
DebugMenu::~DebugMenu()
{
	std::vector< std::pair< std::string, DebugMenuEntryBase* > >::iterator oIt = m_oDebugMenu.begin();
	while( oIt != m_oDebugMenu.end() )
	{
		delete (*oIt).second;
		++oIt;
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
DebugMenu::DebugMenu()
{
	m_oCurrentPosition = 0;
	m_fTimeSinceLastUpdate = TIME_BEFORE_UPDATE + 1.0f;
	m_iFlags = 0;
}

void DebugMenu::Increment()
{
	m_oDebugMenu[ m_oCurrentPosition ].second->Increment();
}

void DebugMenu::Decrement()
{
	m_oDebugMenu[ m_oCurrentPosition ].second->Decrement();
}

void DebugMenu::Next()
{
	if( m_oCurrentPosition == m_oDebugMenu.size() -1 )
	{
		m_oCurrentPosition = 0;
		return;
	}
	++m_oCurrentPosition;
}

void DebugMenu::Previous()
{
	if( m_oCurrentPosition == 0 )
	{
		m_oCurrentPosition = m_oDebugMenu.size() -1;
		return;
	}
	--m_oCurrentPosition;
}

void DebugMenu::Update( float fDeltaTime )
{
	bool bChanged = false;
	if( m_fTimeSinceLastUpdate > TIME_BEFORE_UPDATE )
	{
		if( m_iFlags & E_INCREMENT )
		{
			Increment();
			bChanged = true;
		}
		if( m_iFlags & E_DECREMENT )
		{
			Decrement();
			bChanged = true;
		}
		if( m_iFlags & E_NEXT )
		{
			Next();
			bChanged = true;
		}
		if( m_iFlags & E_PREVIOUS )
		{
			Previous();
			bChanged = true;
		}
		if( bChanged )
		{
			m_fTimeSinceLastUpdate = 0.0f;
		}
	}
	else
	{
		m_fTimeSinceLastUpdate += fDeltaTime * 1000.0f;
	}
}

void DebugMenu::SetFlag( DebugMenuEnum eFlag, bool bValue )
{
	if( bValue )
		m_iFlags |= eFlag;
	else
	{
		m_iFlags ^= eFlag;
		m_fTimeSinceLastUpdate = TIME_BEFORE_UPDATE + 1.0f;
	}
}