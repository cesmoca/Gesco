/*
 * World.cpp
 *
 *  Created on: 21/02/2011
 *      Author: cesar
 */

#include "World.h"

using namespace cv;
using namespace std;

World::World(): _width(320), _height(240), _ticksLastEvent(0), _window(NULL),
		_renderer(NULL), _frameTexture(NULL) {
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

	_renderer = SDL_CreateRenderer(_window, NULL);
	if (_renderer == NULL) {
		Logger::getInstance()->error(std::string("SDL renderer creation failed: ") + SDL_GetError());
		SDL_DestroyWindow(_window);
		_window = NULL;
		SDL_Quit();
		return false;
	}

	_frameTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGB24,
			SDL_TEXTUREACCESS_STREAMING, _width, _height);
	if (_frameTexture == NULL) {
		Logger::getInstance()->error(std::string("SDL texture creation failed: ") + SDL_GetError());
		SDL_DestroyRenderer(_renderer);
		_renderer = NULL;
		SDL_DestroyWindow(_window);
		_window = NULL;
		SDL_Quit();
		return false;
	}

	_ticksLastEvent = SDL_GetTicks();

	Logger::getInstance()->out("World succesfully loaded!");
	return true;
}

void World::drawWorld() {
	//Draw the camera frame
	Mat originalFrame;
	Mat rgbFrame;
	cv::Mat* frame = VideoFactory::getInstance()->getMainCamera().getLastFrame();

	if (frame != NULL && !frame->empty()) {
		resize(*frame, originalFrame, Size(_width, _height));

		if (originalFrame.channels() == 3) {
			cvtColor(originalFrame, rgbFrame, COLOR_BGR2RGB);
		} else if (originalFrame.channels() == 4) {
			cvtColor(originalFrame, rgbFrame, COLOR_BGRA2RGB);
		} else {
			cvtColor(originalFrame, rgbFrame, COLOR_GRAY2RGB);
		}

		SDL_UpdateTexture(_frameTexture, NULL, rgbFrame.data, (int) rgbFrame.step);
		SDL_RenderClear(_renderer);
		SDL_RenderTexture(_renderer, _frameTexture, NULL, NULL);
		SDL_RenderPresent(_renderer);
	}

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
	if (_frameTexture != NULL) {
		SDL_DestroyTexture(_frameTexture);
		_frameTexture = NULL;
	}
	if (_renderer != NULL) {
		SDL_DestroyRenderer(_renderer);
		_renderer = NULL;
	}
	if (_window != NULL) {
		SDL_DestroyWindow(_window);
		_window = NULL;
	}
	SDL_Quit();
}
