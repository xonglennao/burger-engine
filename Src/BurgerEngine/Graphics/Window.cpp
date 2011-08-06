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
bool Window::Initialize( const unsigned int a_iWindowWidth, const unsigned int a_iWindowHeight, bool bFullScreen )
{
	//Set Up parameters
	sf::WindowSettings oSettings;
	// Request a 24 bits depth buffer
	oSettings.DepthBits         = 24; 
	// Request a 8 bits stencil buffer
	oSettings.StencilBits       = 8;  
	// Request 2 levels of antialiasing
	//oSettings.AntialiasingLevel = 2;  

	unsigned int m_eWindowStyle;
	if(bFullScreen)
	{
		m_eWindowStyle = sf::Style::Fullscreen;
	}
	else
	{
		m_eWindowStyle = sf::Style::Resize | sf::Style::Close;
	}
	m_pDriverWindow = new sf::Window();
	m_pDriverWindow->Create(sf::VideoMode(a_iWindowWidth,a_iWindowHeight,32),"BurgerDemo", m_eWindowStyle, oSettings);

	m_i2Size = vec2( static_cast<float>( a_iWindowWidth) , static_cast<float>( a_iWindowHeight ) );

	SetActive(true);


	return true;
}


//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
vec2 const& Window::GetWindowSize() const
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