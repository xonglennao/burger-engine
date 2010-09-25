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
#include "CommonGraphics.h"
#include "BurgerEngine/External/Math/Vector.h"
#include "BurgerEngine/External/SFML/Window.hpp"

class Window
{
public:

	/// \brief default constructor
	Window();

	/// \brief	Initial set up of the Window Application
	bool Initialize( const unsigned int a_iWindowWidth, const unsigned int a_iWindowHeight );

	/// \brief	Clear Buffer and such for a clean exit
	void Terminate();


	/// \brief	SWap front to back buffer + Check Time for FPS
	void Display();

	/// \brief GetWindow size
	vec2 const& GetWindowSize() const;

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

	/// Size of the window
	/// \todo this info is already contain in the implemented window (driver)
	/// but when we will try to separate or create our own window it will be useful
	vec2 m_i2Size;
};

#endif //__WINDOW_H__
