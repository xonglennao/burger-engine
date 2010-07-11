/*************************************
*
*		BurgerEngine Project
*		
*		Created :	05/07/10
*		Authors :	Franck Letellier
*					Baptiste Malaga
*		Contact :   shadervalouf@googlegroups.com
*
**************************************/

#ifndef __WINDOW_H__
#define __WINDOW_H__

/// \name	Window.h
/// \brief	The application of the window

/// TODO dirty
#include "BurgerEngine/External/SFML/Window.hpp"

class Window
{
public:

	/// \brief default constructor
	Window();

	/// \brief	Initial set up of the Window Application
	bool Init();

	/// \brief Swap front and back buffer (avoid flickering)
	void SwapBuffer();

	/// \brief	Clear Buffer and such for a clean exit
	void Terminate();

	/// \brief	Get the plateform specific window
	sf::Window & GrabDriverWindow() const {return *m_pDriverWindow;}

private:

	sf::Window* m_pDriverWindow;
};



#endif //__WINDOW_H__