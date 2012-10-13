/*************************************
*
*		BurgerEngine Project
*		
*		Created :	29/09/12
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __GAMEPLAYMANAGER_H__
#define __GAMEPLAYMANAGER_H__

#include "BurgerEngine/Base/Singleton.h"

class PlayerComponent;

class GameplayManager: public Singleton<GameplayManager>
{
	friend class Singleton<GameplayManager>;

	public:
		void SetCurrentPlayer( PlayerComponent* pPlayer ){ m_pCurrentPlayer = pPlayer; };
		PlayerComponent const * const GetCurrentPlayer( PlayerComponent* pPlayer ) const {return  m_pCurrentPlayer; };
		PlayerComponent* GetCurrentPlayer() {return  m_pCurrentPlayer; };

	private:
		PlayerComponent * m_pCurrentPlayer;

};


#endif //__TEXTUREMANAGER_H__