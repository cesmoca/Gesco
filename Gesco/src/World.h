/*
 * World.h
 *
 *  Created on: 21/02/2011
 *      Author: cesar
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <SDL.h>

#include "VideoFactory.h"
#include "Logger.h"
#include "Singleton.h"

#define FPS 15


class World: public Singleton<World> {
	int _width;
	int _height;
	Uint32 _ticksLastEvent;


public:
	World();
	virtual ~World();
	void initWorld(int width, int height);
	void drawWorld();

	int getWidth();
	int getHeight();
};


#endif /* WORLD_H_ */
