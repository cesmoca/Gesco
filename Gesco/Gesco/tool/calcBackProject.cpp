/**
 * @file BackProject_Demo2.cpp
 * @brief Sample code for backproject function usage ( a bit more elaborated )
 * @author OpenCV team
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/// Global Variables
Mat src; Mat hsv;
Mat mask;
cv::VideoCapture* cam;

int width = 320;
int height = 240;

int lo = 35; int up = 35;
const char* window_image = "Source image";

/// Function Headers
void Hist_and_Backproj( );
void pickPoint (int event, int x, int y, int, void* );

/**
 * @function main
 */
int main( int, char** argv )
{
  // Create the videosource
  cv::VideoCapture* cam = new cv::VideoCapture(0);
  

  if(!cam->isOpened()){
    throw "Camera not found exception";
  }

  /// Read the image
  //src = imread( argv[1], 1 );
  Mat frame;
  for(int i=0; i<25; i++) *cam >> frame;

  resize(frame, src, Size(width, height));

  cam->release();

  /// Transform it to HSV
  cvtColor( src, hsv, COLOR_BGR2HSV );

  /// Show the image
  namedWindow( window_image, WINDOW_AUTOSIZE );
  imshow( window_image, src );

  /// Set Trackbars for floodfill thresholds
  createTrackbar( "Low thresh", window_image, &lo, 255, 0 );
  createTrackbar( "High thresh", window_image, &up, 255, 0 );
  /// Set a Mouse Callback
  setMouseCallback( window_image, pickPoint, 0 );

  waitKey(0);
  return 0;
}

/**
 * @function pickPoint
 */
void pickPoint (int event, int x, int y, int, void* )
{
  if( event != EVENT_LBUTTONDOWN )
    { return;}

  // Fill and get the mask
  Point seed = Point( x, y );

  int newMaskVal = 255;
  Scalar newVal = Scalar( 120, 120, 120 );

  int connectivity = 8;
  int flags = connectivity + (newMaskVal << 8 ) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;

  Mat mask2 = Mat::zeros( src.rows + 2, src.cols + 2, CV_8UC1 );
  floodFill( src, mask2, seed, newVal, 0, Scalar( lo, lo, lo ), Scalar( up, up, up), flags );
  mask = mask2( Range( 1, mask2.rows - 1 ), Range( 1, mask2.cols - 1 ) );

  imshow( "Mask", mask );

  Hist_and_Backproj( );
}

/**
 * @function Hist_and_Backproj
 */
void Hist_and_Backproj( )
{
  MatND hist;
  int h_bins = 30; int s_bins = 32;
  int histSize[] = { h_bins, s_bins };

  float h_range[] = { 0, 179 };
  float s_range[] = { 0, 255 };

  const float* ranges[] = { h_range, s_range };

  int channels[] = { 0, 1 };

  /// Get the Histogram and normalize it
  calcHist( &hsv, 1, channels, mask, hist, 2, histSize, ranges, true, false );

  normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );

  // Save histogram
  cv::FileStorage fs("histogram0.yml", cv::FileStorage::WRITE);
  if (!fs.isOpened()) {std::cout << "unable to open file storage!" << std::endl;}
  else{
    fs << "histogram" << hist;
    fs.release();
  }
  /// Get Backprojection
  MatND backproj;
  calcBackProject( &hsv, 1, channels, hist, backproj, ranges, 1, true );

  /// Draw the backproj
  imshow( "BackProj", backproj );

  // Get contours and save them
  vector<vector<Point> > contours;
  findContours(mask, contours, RETR_EXTERNAL,
	       CHAIN_APPROX_SIMPLE);

  Mat contoursImg = Mat::zeros(mask.size(), CV_8UC3);
  drawContours(contoursImg, contours, -1, Scalar(255, 255, 255), 2);
  imshow("Contours", contoursImg);

  if(contours.size()){
    // Save contour
    fs = cv::FileStorage("gesture0.yml", cv::FileStorage::WRITE);
    if (!fs.isOpened()) {std::cout << "unable to open file storage!" << std::endl;}
    else{
      fs << "gesture"<<"["<<contours.at(0)<<"]";
      fs.release();
      cout<<"Contours with "<<contours.at(0).size()<<" points."<<endl;
    }
  }else{
    cout<<"No contours found :("<<endl;
  }

}
