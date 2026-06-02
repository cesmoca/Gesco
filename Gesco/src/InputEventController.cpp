/*
 * InputEventController.cpp
 *
 *  Created on: 20/02/2011
 *      Author: cesar
 */

#include "InputEventController.h"

InputEventController::InputEventController() {

}

void InputEventController::pollEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		//Mouse events
		case SDL_EVENT_MOUSE_MOTION:
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
			break;
			//Keyboard events
		case SDL_EVENT_KEY_DOWN:
			std::cout<<"Tecla pulsada"<<std::endl;
			if(event.key.key==SDLK_ESCAPE) EndController::getInstance()->end();
			break;
		case SDL_EVENT_KEY_UP:
			break;
			//Quit event
		case SDL_EVENT_QUIT:
			EndController::getInstance()->end();
			break;
		}
	}
}


InputEventController::~InputEventController() {
}
