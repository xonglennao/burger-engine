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

	/// \brief	SWap front to back buffer + Check Time for FPS
	void Display();

	/// \brief Resize the window
	bool Resize(unsigned int a_uHeight, unsigned int a_uWidth);

	/// \brief	Clear Buffer and such for a clean exit
	void Terminate();


	/// \group Grabber
	/// \{
	/// \brief	Get the plateform specific window
	sf::Window& GrabDriverWindow() const {return *m_pDriverWindow;}
	/// \}

	/// \group Getter
	/// \{
	bool IsActive(){return m_bIsActive;}
	/// \}

	/// \group Setter
	/// \{
	void SetActive(bool a_bActive){m_bIsActive = a_bActive;}
	/// \}

private:

	/// \brief Internal buffer swap(avoid flickering)
	void _SwapBuffer();

private:

	sf::Window* m_pDriverWindow;

	bool m_bIsActive;
};



#endif //__WINDOW_H__