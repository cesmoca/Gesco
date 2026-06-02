/*
 * World.cpp
 *
 *  Created on: 21/02/2011
 *      Author: cesar
 */

#include "World.h"

using namespace cv;
using namespace std;

World::World(): _width(320), _height(240), _ticksLastEvent(0), _window(NULL) {
}

bool World::initWorld(int width, int height) {
	Logger::getInstance()->out("Initializing world...");

	_width = width;
	_height = height;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		Logger::getInstance()->error(std::string("SDL initialization failed: ") + SDL_GetError());
		return false;
	}

	_window = SDL_CreateWindow("Gesco", _width, _height, 0);
	if (_window == NULL) {
		Logger::getInstance()->error(std::string("SDL window creation failed: ") + SDL_GetError());
		SDL_Quit();
		return false;
	}

	_ticksLastEvent = SDL_GetTicks();

	Logger::getInstance()->out("World succesfully loaded!");
	return true;
}

void World::drawWorld() {
	//Draw the camera frame
	//Mat originalFrame;
	//cv::Mat* frame = VideoFactory::getInstance()->getMainCamera().getLastFrame();
	//resize(*frame, originalFrame,Size(_width,_height));

	//imshow("OriginalFrame", originalFrame);



	// Wait time
	Uint64 ticksNow = SDL_GetTicks();
	Uint64 ticksDiff = ticksNow - _ticksLastEvent;

	Uint64 frameTime = 1000 / FPS;
	if (ticksDiff < frameTime) {
		SDL_Delay((Uint32)(frameTime - ticksDiff));
	}

	_ticksLastEvent = SDL_GetTicks();
}




int World::getWidth() {
	return _width;
}

int World::getHeight() {
	return _height;
}


World::~World() {
	if (_window != NULL) {
		SDL_DestroyWindow(_window);
		_window = NULL;
	}
	SDL_Quit();
}
