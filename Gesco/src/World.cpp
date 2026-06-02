/*
 * World.cpp
 *
 *  Created on: 21/02/2011
 *      Author: cesar
 */

#include "World.h"

using namespace cv;
using namespace std;

World::World():  _width(320), _height(240), _ticksLastEvent(0){
}

void World::initWorld(int width, int height) {
	Logger::getInstance()->out("Initializing world...");

	_width = width;
	_height = height;

	//SDL_Init(SDL_INIT_EVERYTHING);
	////SDL_SetVideoMode(width, height,24,0);

	//// Original frame
	//namedWindow("OriginalFrame", WINDOW_AUTOSIZE );
	//cv::moveWindow("OriginalFrame", 0, 0);


	//_ticksLastEvent = SDL_GetTicks();


	Logger::getInstance()->out("World succesfully loaded!");
}

void World::drawWorld() {
	////Draw the camera frame
	//Mat originalFrame;
	//cv::Mat* frame = VideoFactory::getInstance()->getMainCamera().getLastFrame();
	//resize(*frame, originalFrame,Size(_width,_height));

	//imshow("OriginalFrame", originalFrame);



	//// Wait time
	//Uint32 ticksNow = SDL_GetTicks();
	//Uint32 ticksDiff = ticksNow - _ticksLastEvent;

	//int waitTime = 1000/FPS - ticksDiff;

	//if(waitTime > 0) {
	//	waitKey(waitTime);
	//}

	//_ticksLastEvent = ticksNow;
	//waitKey(1);
}




int World::getWidth() {
	return _width;
}

int World::getHeight() {
	return _height;
}


World::~World() {
}
