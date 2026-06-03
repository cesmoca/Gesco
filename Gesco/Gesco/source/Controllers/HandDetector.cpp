/*
 * HandDetector.cpp
 *
 *  Created on: 20/02/2011
 *      Author: cesar
 */

#include <Controllers/HandDetector.h>

using namespace cv;
using namespace std;

HandDetector::HandDetector(Hand* hand, int width, int height) :
		_hand(hand) {

	_width = width;
	_height = height;
	loadSkinHistograms();
}

void HandDetector::detect(cv::Mat& frame) {

	// Grab the frame
	if (!storeFrames(frame))
		return;

	// Clear hand frames
	_hand->handsMasks().clear();

	// Get difference mask
	Mat differenceMask;
	threshold(_movFrame, differenceMask, THRES_DIFF_MASK, 255, THRESH_BINARY);

	// Get skin mask
	vector<Mat> skinMasks(_skinHistograms.size());
	getSkinMask(skinMasks);

	// IMPROVE THIS PART! THERE ARE DIFFERENT SKIN HISTOGRAMS! WTF!!!
	// Get background mask
	Mat backgroundMask;
	//getBackgroundMask(_frameBlur, backgroundMask, diffSkinMask);
	getBackgroundMask(_frameBlur, backgroundMask);

	// For each histogram
	for (unsigned int i = 0; i < _skinHistograms.size(); i++) {

		Mat diffSkinMask = differenceMask | skinMasks.at(i);

		Mat erodedBackgroundMask;
		erode((backgroundMask & diffSkinMask), erodedBackgroundMask, Mat());

		// Find seed! Movement + skin mask :)
		findMovingSeedPoint();
		if (_hand->seedPoint().x < 0 || _hand->seedPoint().y < 0
				|| _hand->seedPoint().x >= skinMasks.at(i).cols
				|| _hand->seedPoint().y >= skinMasks.at(i).rows) {
			return;
		}
		if (!skinMasks.at(i).at<uchar>(_hand->seedPoint().y,
				_hand->seedPoint().x)
				&& !interpolateSeedPoint(skinMasks.at(i), _frame)) {
			return;
		}

		Mat floodedFrame;
		getFloodedMask(erodedBackgroundMask, floodedFrame);

		_hand->handsMasks().push_back(floodedFrame);

	}
}

void HandDetector::getSkinMask(std::vector<cv::Mat>& masks) {

	float h_range[] = { 0, 180 };
	float s_range[] = { 0, 255 };
	const float* ranges[] = { h_range, s_range };
	int channels[] = { 0, 1 };

	// Get Backprojection
	MatND backProj;
	for (unsigned int i = 0; i < _skinHistograms.size(); i++) {

		calcBackProject(&_frameHSV, 1, channels, _skinHistograms.at(i),
				backProj, ranges, 1, true);

		threshold(backProj, masks.at(i), BACK_PROJ_THRES, 255, THRESH_BINARY);
	}

	return;
}

// Use accumulator!
void HandDetector::getBackgroundMask(cv::Mat& frame, cv::Mat& backgroundMask) {

	// Dont do that where the man moves!
	//Mat maskInv;
	//threshold(mask, maskInv, 180, 255, THRESH_BINARY_INV);

	//accumulateWeighted(frame, _backgroundAcc, ACCUMULATOR_SPEED, maskInv);
	accumulateWeighted(frame, _backgroundAcc, ACCUMULATOR_SPEED);

	Mat backgroundImage;
	convertScaleAbs(_backgroundAcc, backgroundImage);

	// Difference frame
	backgroundMask = Mat::zeros(_height, _width, CV_8UC1);

	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			Vec3b framePixel = frame.at<Vec3b>(i, j);
			Vec3b backgroundPixel = backgroundImage.at<Vec3b>(i, j);
			float diff = pow((float) framePixel[0] - backgroundPixel[0], 2)
					+ pow((float) framePixel[1] - backgroundPixel[1], 2)
					+ pow((float) framePixel[2] - backgroundPixel[2], 2);

			backgroundMask.at<uchar>(i, j) =
					(diff > THRES_ACCUMULATOR) ? 255 : 0;
		}
	}
}

void HandDetector::getFloodedMask(cv::Mat& frame, cv::Mat& floodedMask) {
	int newMaskVal = 255;
	int lo = FLOODFILL_PARAM;
	int up = FLOODFILL_PARAM;
	int connectivity = 4;
	int flags = connectivity + (newMaskVal << 8) + FLOODFILL_FIXED_RANGE
			+ FLOODFILL_MASK_ONLY;

	try {
		Mat mask;
		mask = Mat::zeros(_height + 2, _width + 2, CV_8UC1);

		floodFill(frame, mask, _hand->seedPoint(), Scalar(), 0,
				Scalar(lo, lo, lo), Scalar(up, up, up), flags);

		floodedMask = Mat(_height, _width, CV_8UC1);

		for (int i = 0; i < _height; i++) {
			for (int j = 0; j < _width; j++) {
				floodedMask.at<uchar>(i, j) = mask.at<uchar>(i + 1, j + 1);
			}
		}

	} catch (cv::Exception& e) {
	}
}

bool HandDetector::interpolateSeedPoint(cv::Mat& skinMask,
		cv::Mat& frameWithMask) {

	// Inside a KERNEL, look for a "bunch of white pixels", defined by LOCAL_KERNEL
	for (int i = _hand->seedPoint().x - SEED_POINT_KERNEL;
			i < _hand->seedPoint().x + SEED_POINT_KERNEL;
			i = i + SEED_POINT_LOCAL_KERNEL) {
		for (int j = _hand->seedPoint().y - SEED_POINT_KERNEL;
				j < _hand->seedPoint().y + SEED_POINT_KERNEL;
				j = j + SEED_POINT_LOCAL_KERNEL) {

			if (frameWithMask.at<Vec3b>(j, i)[0] == 0)
				continue;

			if (i > SEED_POINT_LOCAL_KERNEL && j > SEED_POINT_LOCAL_KERNEL
					&& i < skinMask.cols - SEED_POINT_LOCAL_KERNEL
					&& j < skinMask.rows - SEED_POINT_LOCAL_KERNEL) {
				if (skinMask.at<uchar>(j, i)
						&& skinMask.at<uchar>(j - SEED_POINT_LOCAL_KERNEL, i)
						&& skinMask.at<uchar>(j + SEED_POINT_LOCAL_KERNEL, i)
						&& skinMask.at<uchar>(j, i - SEED_POINT_LOCAL_KERNEL)
						&& skinMask.at<uchar>(j, i + SEED_POINT_LOCAL_KERNEL)) {
					// Check sizes
					if (_hand->seedPoint().x < 0 || _hand->seedPoint().y < 0
							|| _hand->seedPoint().x >= _width
							|| _hand->seedPoint().y >= _height) {
						return false;
					}
					_hand->seedPoint().x = i;
					_hand->seedPoint().y = j;
					return true;
				}
			}
		}
	}

	return false;

}

bool HandDetector::findMovingSeedPoint() {

	int nPoints = 0;
	Point candidate(0, 0);

	for (int i = 0; i < _movFrame.cols; i++) {
		for (int j = 0; j < _movFrame.rows; j++) {
			if (_movFrame.at<uchar>(j, i) > MIN_DIFF_MOV) {
				nPoints++;
				candidate += Point(i, j);
			}
		}
	}

	if (nPoints > MIN_DIFF_POINTS) {
		candidate.x /= nPoints;
		candidate.y /= nPoints;

		_hand->seedPoint() = candidate;
		return true;
	}

	return false;
}

bool HandDetector::storeFrames(cv::Mat& frame) {

	if (_frame.rows != 0) {
		// Saving to previous frame. In GRAY to save time
		cvtColor(_frame, _prevframe, COLOR_BGR2GRAY);

		//Mat preEqual;
		resize(frame, _frame, Size(_width, _height));
		//equalizeHist(preEqual, _originalFrame);

		// Convert it to gray
		cvtColor(_frame, _frameGray, COLOR_BGR2GRAY);

		// Difference frame
		absdiff(_frameGray, _prevframe, _movFrame);

		// Gaussian Blur
		Mat auxsrc;
		Size GAUS_KERNEL(9, 9);
		double GAUS_SIGMA = 5;

		// Color blur
		GaussianBlur(_frame, _frameBlur, GAUS_KERNEL, GAUS_SIGMA);

		// Gray blur
		GaussianBlur(_frameGray, _frameGrayBlur, GAUS_KERNEL, GAUS_SIGMA);

		// Convert to HSV
		cvtColor(_frameBlur, _frameHSV, COLOR_BGR2HSV);
		split(_frameHSV, _vectorFrameHSV);

		return true;
	} else {
		// Initializations here :)
		_backgroundAcc = Mat::zeros(_height, _width, CV_64FC3);

		resize(frame, _frame, Size(_width, _height));

		// Gaussian Blur
		Mat auxsrc;
		Size GAUS_KERNEL(9, 9);
		double GAUS_SIGMA = 5;

		// Color blur
		GaussianBlur(_frame, _frameBlur, GAUS_KERNEL, GAUS_SIGMA);

		// Initialize the accumulator with the first frame TODO
		for (int i = 0; i < 2000; i++)
			accumulateWeighted(_frameBlur, _backgroundAcc, ACCUMULATOR_SPEED);

		return false;
	}

}

void HandDetector::loadSkinHistograms() {
	int histN = 1;
	cout << "Loading skin histograms... " << flush;
	while (true) {
		cv::Mat histogram;
		stringstream ss;

		ss << "data/histograms/histogram" << histN << ".yml";
		cv::FileStorage fs(ss.str(), cv::FileStorage::READ);
		if (!fs.isOpened()) {
			break;
		} else {
			fs["histogram"] >> histogram;
			_skinHistograms.push_back(histogram);
			histN++;
			fs.release();
		}
	}
	cout << "Read " << (histN - 1) << " skin histogramms!" << endl;
}

void HandDetector::setDebugController(DebugController* debugController) {
	_debugController = debugController;
}

HandDetector::~HandDetector() {
}
