#include "Window.h"
#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Input/EventManager.h"
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
Window::Window()
{
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
bool Window::Initialize( const float a_fWindowWidth, const float a_fWindowHeight )
{
	//Set Up parameters
	sf::WindowSettings oSettings;
	// Request a 24 bits depth buffer
	oSettings.DepthBits         = 24; 
	// Request a 8 bits stencil buffer
	oSettings.StencilBits       = 8;  
	// Request 2 levels of antialiasing
	oSettings.AntialiasingLevel = 2;  

	unsigned int m_eWindowStyle = sf::Style::Resize | sf::Style::Close;

	m_pDriverWindow = new sf::Window();
	m_pDriverWindow->Create(sf::VideoMode(a_fWindowWidth,a_fWindowHeight,32),"BurgerDemo", m_eWindowStyle, oSettings);

	m_i2Size.set(a_fWindowWidth,a_fWindowHeight);

	SetActive(true);


	return true;
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
osg::Vec2s const& Window::GetWindowSize() const
{
	return m_i2Size;
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Window::Display()
{
	/// \todo Timer class to FPS
	/// Also right know we are just calling the sfml window
	if (IsActive())
	{
		m_pDriverWindow->Display();
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Window::_SwapBuffer()
{
	//m_pDriverWindow->
}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void Window::Terminate()
{
	m_pDriverWindow->Close();
	delete m_pDriverWindow;
	m_pDriverWindow = NULL;
}