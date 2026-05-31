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

	// Gesture recognized
	namedWindow("GestureRecognized", WINDOW_AUTOSIZE);
	moveWindow("GestureRecognized", 0, _height + 60);

	int x = _width + 15;
	int y = 0;

	for (int i = 0; i < TEST_WINDOWS; i++) {
		stringstream ss;
		ss << "Test" << i << "Window";
		namedWindow(ss.str(), WINDOW_AUTOSIZE);
		moveWindow(ss.str(), x, y);
		x += _width + 15;
		if (x > (1366 - _width)) {
			x = _width + 15;
			y += _height + 60;
		}
	}

	for (int i = 0; i < TEST_WINDOWS; i++)
		_testFramesVector.push_back(Mat());

}

void DebugController::show() {
	for (unsigned int i = 0; i < _testFramesVector.size(); i++) {
		stringstream ss;
		ss << "Test" << i << "Window";
		Mat& testFrame = _testFramesVector.at(i);
		if (testFrame.rows > 0)
			imshow(ss.str(), testFrame);
	}

	// Gesture recognized logic
	if (_timerGestureRecognized > 0) {
		_timerGestureRecognized--;
	} else if (_timerGestureRecognized == 0) {
		_timerGestureRecognized--;
		_gestureRecognizedImg = _noGestureRecognizedImg;
	}

	imshow("GestureRecognized", _gestureRecognizedImg);
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

