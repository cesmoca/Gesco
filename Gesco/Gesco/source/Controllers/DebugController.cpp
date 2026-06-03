/*
 * DebugController.cpp
 *
 *  Created on: 03/05/2013
 *      Author: cesar
 */

#include <Controllers/DebugController.h>

using namespace std;
using namespace cv;

DebugController::DebugController(Hand* hand, int width, int height) :
		_hand(hand), _width(width), _height(height), _timerGestureRecognized(0) {

	_noGestureRecognizedImg = Mat::zeros(Size(_width, _height), CV_8UC1);
	_gestureRecognizedImg = _noGestureRecognizedImg;

	for (int i = 0; i < TEST_WINDOWS; i++)
		_testFramesVector.push_back(Mat());

}

void DebugController::show() {
	// Gesture recognized logic
	if (_timerGestureRecognized > 0) {
		_timerGestureRecognized--;
	} else if (_timerGestureRecognized == 0) {
		_timerGestureRecognized--;
		_gestureRecognizedImg = _noGestureRecognizedImg;
	}

}

void DebugController::setTestFrame(unsigned int n, cv::Mat& mat) {
	if (n < 0 || n >= _testFramesVector.size()) {
		cout << "TestFrame number incorrect!" << endl;
		return;
	}
	mat.copyTo(_testFramesVector.at(n));
}

void DebugController::setGestureRecognized(cv::Mat& gestureImg) {
	_timerGestureRecognized = SHOW_GESTURE_TIME
	;
	_gestureRecognizedImg = gestureImg;

}

const std::vector<cv::Mat>& DebugController::getTestFrames() const {
	return _testFramesVector;
}

const cv::Mat& DebugController::getGestureRecognizedImg() const {
	return _gestureRecognizedImg;
}

