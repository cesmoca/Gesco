/*
 * VideoSource.cpp
 *
 *  Created on: 20/02/2011
 *      Author: cesar
 */

#include "VideoSource.h"

using namespace std;

VideoSource::VideoSource(const std::string& name, const int& nDevice) :
		_lastFrame(NULL), _name(name), _cam(NULL), _width(640), _height(480),
		_fps(0) {
	cv::VideoCapture* c = new cv::VideoCapture(nDevice);
	_cam = c;

	if (!_cam->isOpened()) {
		throw "Camera not found exception: " + name;
	}

	_lastFrame = new cv::Mat();

	_cam->set(cv::CAP_PROP_FRAME_WIDTH, _width);
	_cam->set(cv::CAP_PROP_FRAME_HEIGHT, _height);

	_width = _cam->get(cv::CAP_PROP_FRAME_WIDTH);
	_height = _cam->get(cv::CAP_PROP_FRAME_HEIGHT);
	//cout<<"Theoretical fps: "<<_cam->get(cv::CAP_PROP_FPS)<<endl;
	//_fps = -1;

	// Getting a first frame
	cv::Mat frame;
	*_cam >> frame;

}

cv::Mat* VideoSource::grabFrame() {
	*_cam >> *_lastFrame;

	return _lastFrame;
}

cv::Mat* VideoSource::getLastFrame() {
	if (_lastFrame != NULL)
		return _lastFrame;
	throw "No frame found exception!";
}

std::string& VideoSource::getName() {
	return _name;
}

int VideoSource::getWidth() {
	return _width;
}

int VideoSource::getHeight() {
	return _height;
}

int VideoSource::getFps() {
	return _fps;
}

VideoSource::~VideoSource() {
#ifndef WIN32
	delete _cam;
#endif

	if (_lastFrame != NULL)
		delete _lastFrame;
}
