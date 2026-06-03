/*
 * DebugController.h
 *
 *  Created on: 03/05/2013
 *      Author: cesar
 */

#ifndef DEBUGCONTROLLER_H_
#define DEBUGCONTROLLER_H_

#include <Kernel/Hand.h>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <vector>

#define TEST_WINDOWS 6

#define SHOW_GESTURE_TIME 25;


class DebugController {
	Hand* _hand;

	int _width, _height;

	cv::Mat _gestureRecognizedImg;
	cv::Mat _noGestureRecognizedImg;

	int _timerGestureRecognized;

	std::vector<cv::Mat> _testFramesVector;

public:

	DebugController(Hand* hand, int width, int height);

	void show();

	void setTestFrame(unsigned int n, cv::Mat& mat);
	void setGestureRecognized(cv::Mat& gestureImg);

	const std::vector<cv::Mat>& getTestFrames() const;
	const cv::Mat& getGestureRecognizedImg() const;

};

#endif /* DEBUGCONTROLLER_H_ */
