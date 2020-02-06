#include "Control.h"
#include <iostream>

void Control::events() {
	
	drawSys.window->setKeyRepeatEnabled(false);
	mousePrev = mouse;
	sf::Event event;
	mouse.delta = 0;
	while (drawSys.window->pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Closed:
			stop = 1;
			drawSys.window->close();
			break;
		case sf::Event::KeyPressed:
			for (int k : keyMatches[getKey(event.key.code)]) {
				keys[k] = 1;
			}
			break;
		case sf::Event::KeyReleased:
			for (int k : keyMatches[getKey(event.key.code)]) {
				keys[k] = 0;
			}
			break;
		case sf::Event::MouseMoved:
			mouse.pos.x = event.mouseMove.x;
			mouse.pos.y = event.mouseMove.y;
			break;
		case sf::Event::MouseWheelMoved:
			mouse.delta = event.mouseWheel.delta;
			break;
		case sf::Event::MouseButtonPressed:
			mouse.state = 1;
			break;
		case sf::Event::MouseButtonReleased:
			mouse.state = 0;
			break;
		case sf::Event::JoystickButtonPressed:
			joystick.get(event.joystickButton.button) = 1;
			break;
		case sf::Event::JoystickButtonReleased:
			joystick.get(event.joystickButton.button) = 0;
			break;
		case sf::Event::JoystickMoved:
			if(event.joystickMove.axis == 0)
				joystick.pos.x = event.joystickMove.position;
			else
				joystick.pos.y = event.joystickMove.position;
			break;
		}
	}
}