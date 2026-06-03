/*
 * VideoSource.cpp
 *
 *  Created on: 20/02/2011
 *      Author: cesar
 */

#include "VideoSource.h"

#include <sstream>

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

	std::stringstream requestedMode;
	requestedMode << "Opening camera '" << _name << "' on device " << nDevice
			<< ". Requested mode: " << _width << "x" << _height;
	Logger::getInstance()->out(requestedMode.str());

	bool widthSet = _cam->set(cv::CAP_PROP_FRAME_WIDTH, _width);
	bool heightSet = _cam->set(cv::CAP_PROP_FRAME_HEIGHT, _height);

	_width = _cam->get(cv::CAP_PROP_FRAME_WIDTH);
	_height = _cam->get(cv::CAP_PROP_FRAME_HEIGHT);
	_fps = _cam->get(cv::CAP_PROP_FPS);

	std::stringstream selectedMode;
	selectedMode << "Camera backend: " << _cam->getBackendName()
			<< ". Width set: " << (widthSet ? "yes" : "no")
			<< ", height set: " << (heightSet ? "yes" : "no")
			<< ". Selected mode: " << _width << "x" << _height
			<< " @ " << _fps << " FPS";
	Logger::getInstance()->out(selectedMode.str());

	Logger::getInstance()->warning(
			"OpenCV VideoCapture does not expose a portable list of supported camera modes.");

	// Getting a first frame
	cv::Mat frame;
	*_cam >> frame;
	if (!frame.empty()) {
		std::stringstream firstFrame;
		firstFrame << "First captured frame: " << frame.cols << "x"
				<< frame.rows << ", channels: " << frame.channels();
		Logger::getInstance()->out(firstFrame.str());
	}

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
	std::cout << "Destroying VideoSource '" << _name << "'..." << std::endl;
	delete _cam;
	if (_lastFrame != NULL)
		delete _lastFrame;
}
