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
		_renderer(NULL), _frameTexture(NULL), _debugController(NULL) {
}

bool World::initWorld(int width, int height) {
	Logger::getInstance()->out("Initializing world...");

	_width = width;
	_height = height;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		Logger::getInstance()->error(std::string("SDL initialization failed: ") + SDL_GetError());
		return false;
	}

	int windowWidth = (_debugController != NULL) ? _width * 2 : _width;
	int windowHeight = _height;

	_window = SDL_CreateWindow("Gesco", windowWidth, windowHeight, 0);
	if (_window == NULL) {
		Logger::getInstance()->error(std::string("SDL window creation failed: ") + SDL_GetError());
		SDL_Quit();
		return false;
	}

	int actualWindowWidth = 0;
	int actualWindowHeight = 0;
	int pixelWidth = 0;
	int pixelHeight = 0;
	SDL_GetWindowSize(_window, &actualWindowWidth, &actualWindowHeight);
	SDL_GetWindowSizeInPixels(_window, &pixelWidth, &pixelHeight);

	std::stringstream windowInfo;
	windowInfo << "SDL window size: " << actualWindowWidth << "x" << actualWindowHeight
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
	if (_debugController != NULL) {
		size_t debugCount = _debugController->getTestFrames().size() + 1;
		_debugTextures.assign(debugCount, NULL);
		_debugRgbFrames.resize(debugCount);
	}

	_ticksLastEvent = SDL_GetTicks();

	Logger::getInstance()->out("World succesfully loaded!");
	return true;
}

void World::drawWorld() {
	//Draw the camera frame
	cv::Mat* frame = VideoFactory::getInstance()->getMainCamera().getLastFrame();

	if (frame != NULL && !frame->empty()) {
		resize(*frame, _originalFrame, Size(_width, _height));
		SDL_RenderClear(_renderer);
		SDL_FRect cameraDestination = {0, 0, (float) _width, (float) _height};
		renderMatToTexture(_originalFrame, &_frameTexture, _rgbFrame, cameraDestination);
		renderDebugFrames();
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

void World::renderMatToTexture(const cv::Mat& frame, SDL_Texture** texture,
		cv::Mat& rgbFrame, const SDL_FRect& destination) {
	if (frame.empty()) {
		return;
	}

	if (frame.channels() == 3) {
		cvtColor(frame, rgbFrame, COLOR_BGR2RGB);
	} else if (frame.channels() == 4) {
		cvtColor(frame, rgbFrame, COLOR_BGRA2RGB);
	} else {
		cvtColor(frame, rgbFrame, COLOR_GRAY2RGB);
	}

	if (*texture == NULL) {
		*texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGB24,
				SDL_TEXTUREACCESS_STREAMING, rgbFrame.cols, rgbFrame.rows);
	}

	SDL_UpdateTexture(*texture, NULL, rgbFrame.data, (int) rgbFrame.step);
	SDL_RenderTexture(_renderer, *texture, NULL, &destination);
}

void World::renderDebugFrames() {
	if (_debugController == NULL) {
		return;
	}

	const vector<Mat>& testFrames = _debugController->getTestFrames();
	const float panelX = (float) _width;
	const float tileWidth = (float) _width / 2.0f;
	const float tileHeight = (float) _height / 4.0f;

	for (unsigned int i = 0; i < testFrames.size(); i++) {
		if (testFrames.at(i).empty()) {
			continue;
		}

		SDL_FRect destination = {
				panelX + (float) (i % 2) * tileWidth,
				(float) (i / 2) * tileHeight,
				tileWidth,
				tileHeight
		};
		renderMatToTexture(testFrames.at(i), &_debugTextures.at(i),
				_debugRgbFrames.at(i), destination);
	}

	const Mat& gestureFrame = _debugController->getGestureRecognizedImg();
	unsigned int gestureIndex = (unsigned int) testFrames.size();
	if (!gestureFrame.empty() && gestureIndex < _debugTextures.size()) {
		SDL_FRect destination = {
				panelX,
				(float) _height - tileHeight,
				tileWidth,
				tileHeight
		};
		renderMatToTexture(gestureFrame, &_debugTextures.at(gestureIndex),
				_debugRgbFrames.at(gestureIndex), destination);
	}
}




int World::getWidth() {
	return _width;
}

int World::getHeight() {
	return _height;
}

void World::setDebugController(DebugController* debugController) {
	_debugController = debugController;
}

World::~World() {
	for (unsigned int i = 0; i < _debugTextures.size(); i++) {
		if (_debugTextures.at(i) != NULL) {
			SDL_DestroyTexture(_debugTextures.at(i));
			_debugTextures.at(i) = NULL;
		}
	}
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
