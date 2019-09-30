#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include <cmath>


using namespace cv;
using namespace std;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Canny";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*3, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow( window_name, dst );
 }

// Computes the x component of the gradient vector
// at a given point in a image.
// returns gradient in the x direction
int xGradient(Mat image, int x, int y)
{
    return image.at<uchar>(y-1, x-1) +
                2*image.at<uchar>(y, x-1) +
                 image.at<uchar>(y+1, x-1) -
                  image.at<uchar>(y-1, x+1) -
                   2*image.at<uchar>(y, x+1) -
                    image.at<uchar>(y+1, x+1);
}
 
// Computes the y component of the gradient vector
// at a given point in a image
// returns gradient in the y direction
 
int yGradient(Mat image, int x, int y)
{
    return image.at<uchar>(y-1, x-1) +
                2*image.at<uchar>(y-1, x) +
                 image.at<uchar>(y-1, x+1) -
                  image.at<uchar>(y+1, x-1) -
                   2*image.at<uchar>(y+1, x) -
                    image.at<uchar>(y+1, x+1);
}


/** @function main */
int main( int argc, char** argv )
{
  Mat Imag;
  VideoCapture capture(0);
  for(;;)
    {
    capture >> src;
    //src = imread("/home/vanessa/Documents/VISION/PRACTICA2_1/binarystitching.png");
  /// Load an image
  //src = imread(image[1]);

  if( !src.data )
  { return -1; }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

  /// Convert the image to grayscale
  cvtColor( src, src_gray, COLOR_BGR2GRAY );

  /// Create a window
  //namedWindow("Canny", WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

  /// Show the image
  CannyThreshold(0, 0);

  Mat sobelx;
  Sobel(src_gray, sobelx, CV_32F, 1, 0);

  double minVal, maxVal;
  minMaxLoc(sobelx, &minVal, &maxVal); //find minimum and maximum intensities
  cout << "minVal : " << minVal << endl << "maxVal : " << maxVal << endl;

  Mat draw;
  sobelx.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

  namedWindow("Sobel", WINDOW_AUTOSIZE);
  imshow("Sobel", draw);

  Mat dst;
      int gx, gy, sum;
 
      // Load an image
      dst = src_gray.clone(); 
 
        for(int y = 0; y < src_gray.rows; y++)
            for(int x = 0; x < src_gray.cols; x++)
                dst.at<uchar>(y,x) = 0.0;
 
        for(int y = 1; y < src_gray.rows - 1; y++){
            for(int x = 1; x < src_gray.cols - 1; x++){
                gx = xGradient(src_gray, x, y);
                gy = yGradient(src_gray, x, y);
                sum = abs(gx) + abs(gy);
                sum = sum > 255 ? 255:sum;
                sum = sum < 0 ? 0 : sum;
                dst.at<uchar>(y,x) = sum;
            }
        }
 
        namedWindow("Prewitt");
        imshow("Prewitt", dst);

  /// Wait until user exit program by pressing a key
  int key=waitKey(1);
    if (key==27)
        break;
    }
  return 0;
  }