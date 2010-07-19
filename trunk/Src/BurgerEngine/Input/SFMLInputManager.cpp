#include "SFMLInputManager.h"
#include "EventManager.h"

#include "BurgerEngine/Core/Engine.h"
#include "BurgerEngine/Graphics/Window.h"

#include <SFML/Window.hpp>
//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SFMLInputManager::InitializeInput()
{


}

//--------------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------------
void SFMLInputManager::ProcessEvents()
{
	Engine& rEngine = Engine::GetInstance();
	EventManager const& rEventManager = rEngine.GetEventManager();

	sf::Window & rWindow = rEngine.GetWindow().GrabDriverWindow();
	sf::Event SFMLEvent;
	
	//go through all event since last frame
	// It's SFML specific that we get the window
	while(rWindow.GetEvent(SFMLEvent))
	{
		switch(SFMLEvent.Type)
		{
		case sf::Event::KeyPressed:
			switch (SFMLEvent.Key.Code)
			{
			//case 27
			case sf::Key::Escape:
				rEngine.SetTerminate(true);
				break;
			default:
				rEventManager.DispatchKeyboardDownKeyEvent(SFMLEvent.Key.Code);
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			break;
		case sf::Event::MouseMoved:
			/// If a mus button is clicked, we use ActiveMotion
			rEventManager.DispatchMousePassiveMotion(SFMLEvent.MouseMove.X,SFMLEvent.MouseMove.Y);
			break;

		case sf::Event::Resized:
			rEventManager.DispatchResize(SFMLEvent.Size.Height,SFMLEvent.Size.Width);
			break;
		case sf::Event::Closed:
			rEngine.SetTerminate(true);
			break;
		}
	
	}
}

