/*
 * main.cpp
 *
 *  Created on: 19/02/2011
 *      Author: cesar
 *
 *      Test application
 */
#include "InputEventController.h"
#include "VideoFactory.h"
#include "EndController.h"
#include "World.h"
#include "Logger.h"

#include <Recognizer.h>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

	std::cout << "Full init"<< std::endl;

	try {
		std::cout << "Starting Gesco..." << std::endl;

		VideoFactory::getInstance()->addVideoSource("cam", 0);

		Recognizer* recognizer = new Recognizer(
				VideoFactory::getInstance()->getMainCamera().getWidth(),
				VideoFactory::getInstance()->getMainCamera().getHeight(),
				1.0f,
				true);
		World::getInstance()->setDebugController(recognizer->getDebugController());

		if (!World::getInstance()->initWorld(
				VideoFactory::getInstance()->getMainCamera().getWidth(),
				VideoFactory::getInstance()->getMainCamera().getHeight())) {
			return 1;
		}

		// Main loop
		std::string lastGesture;
		while (EndController::getInstance()->isRunning()) {
		// Grab a frame from the camera!
		VideoFactory::getInstance()->getMainCamera().grabFrame();

		// Poll the events
		InputEventController::getInstance()->pollEvents();

		// Detection Logic
		cv::Mat* frame = VideoFactory::getInstance()->getInstance()->getMainCamera().getLastFrame();
		recognizer->detect(*frame);

		// Print the results, if any
		std::string& result = recognizer->getRecognizedGesture();
		if(result != "" && result != lastGesture) {
			std::cout<<"Recognized gesture: "<<result<<endl;
			lastGesture = result;
		}

		// Draw
		World::getInstance()->drawWorld();
		}

		//Freeing resources
		std::cout << "Freeing resources..." << std::endl;

		World::getInstance()->destroy();
		VideoFactory::getInstance()->destroy();

		return 0;
	} catch (const std::exception& e) {
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return 1;
	} catch (const std::string& e) {
		std::cerr << "Fatal error: " << e << std::endl;
		return 1;
	} catch (const char* e) {
		std::cerr << "Fatal error: " << e << std::endl;
		return 1;
	}

	return 0;
}
