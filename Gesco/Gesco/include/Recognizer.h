/*
 * Recognizer.h
 *
 *  Created on: 01/05/2013
 *      Author: cesar
 */

#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_

#include <Controllers/HandDetector.h>
#include <Controllers/GestureRecognizer.h>
#include <Controllers/DebugController.h>
#include <Kernel/Hand.h>

class Recognizer{

	Hand _hand;

	//cv::Mat _frame;

	DebugController* _debugController;
	HandDetector* _handDetector;
	GestureRecognizer* _gestureRecognizer;

	int _frameWidth;
	int _frameHeight;
	float _scale;

public:
	Recognizer(int width, int height, float scale = 1.0f, bool debug = false);
	~Recognizer();

	void detect(cv::Mat& frame);
	DebugController* getDebugController();
	std::string& getRecognizedGesture();

	// Public interface :)
	int getGesture();

};

#endif /* RECOGNIZER_H_ */
