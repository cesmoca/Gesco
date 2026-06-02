/*
 * World.cpp
 *
 *  Created on: 21/02/2011
 *      Author: cesar
 */

#include "World.h"

#include <sstream>

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

	int windowWidth = 0;
	int windowHeight = 0;
	int pixelWidth = 0;
	int pixelHeight = 0;
	SDL_GetWindowSize(_window, &windowWidth, &windowHeight);
	SDL_GetWindowSizeInPixels(_window, &pixelWidth, &pixelHeight);

	std::stringstream windowInfo;
	windowInfo << "SDL window size: " << windowWidth << "x" << windowHeight
			<< " logical units; " << pixelWidth << "x" << pixelHeight
			<< " physical pixels.";
	Logger::getInstance()->out(windowInfo.str());

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

	_originalFrame.create(_height, _width, CV_8UC3);
	_rgbFrame.create(_height, _width, CV_8UC3);

	_ticksLastEvent = SDL_GetTicks();

	Logger::getInstance()->out("World succesfully loaded!");
	return true;
}

void World::drawWorld() {
	//Draw the camera frame
	cv::Mat* frame = VideoFactory::getInstance()->getMainCamera().getLastFrame();

	if (frame != NULL && !frame->empty()) {
		resize(*frame, _originalFrame, Size(_width, _height));

		if (_originalFrame.channels() == 3) {
			cvtColor(_originalFrame, _rgbFrame, COLOR_BGR2RGB);
		} else if (_originalFrame.channels() == 4) {
			cvtColor(_originalFrame, _rgbFrame, COLOR_BGRA2RGB);
		} else {
			cvtColor(_originalFrame, _rgbFrame, COLOR_GRAY2RGB);
		}

		SDL_UpdateTexture(_frameTexture, NULL, _rgbFrame.data, (int) _rgbFrame.step);
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
