/*
 * World.h
 *
 *  Created on: 21/02/2011
 *      Author: cesar
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <opencv2/opencv.hpp>
#include <SDL3/SDL.h>
#include <vector>

#include <Controllers/DebugController.h>
#include "VideoFactory.h"
#include "Logger.h"
#include "Singleton.h"

#define FPS 15


class World: public Singleton<World> {
	int _width;
	int _height;
	Uint64 _ticksLastEvent;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Texture* _frameTexture;
	cv::Mat _originalFrame;
	cv::Mat _rgbFrame;
	DebugController* _debugController;
	std::vector<SDL_Texture*> _debugTextures;
	std::vector<cv::Mat> _debugRgbFrames;

	void renderMatToTexture(const cv::Mat& frame, SDL_Texture** texture,
			cv::Mat& rgbFrame, const SDL_FRect& destination);
	void renderDebugFrames();


public:
	World();
	virtual ~World();
	bool initWorld(int width, int height);
	void drawWorld();
	void setDebugController(DebugController* debugController);

	int getWidth();
	int getHeight();
};


#endif /* WORLD_H_ */
