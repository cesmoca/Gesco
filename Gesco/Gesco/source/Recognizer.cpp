#include <Recognizer.h>

using namespace std;

Recognizer::Recognizer(int width, int height, float scale, bool debug) {

	_scale = scale;
	if (scale > 1 || scale <= 0) {
		cout<<"Incorrect scale factor. Setting it up to 1.0"<<endl;
		_scale = 1.0;
	}

	_frameWidth = width;
	_frameHeight = height;

	// Hand stores the info :)
	_handDetector = new HandDetector(&_hand, _frameWidth*_scale, _frameHeight*_scale);
	_gestureRecognizer = new GestureRecognizer(&_hand);

	//if (debug) {
	//	_debugController = new DebugController(&_hand, _frameWidth*_scale, _frameHeight*_scale);
	//	_handDetector->setDebugController(_debugController);
	//	_gestureRecognizer->setDebugController(_debugController);
	//}

}

void Recognizer::detect(cv::Mat& frame) {
	_handDetector->detect(frame);
	_gestureRecognizer->detect();

	// Trigger the debug controller :)
	//_debugController->show();
}

Recognizer::~Recognizer() {

}
