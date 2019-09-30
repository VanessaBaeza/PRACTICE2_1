#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
// we're NOT "using namespace std;" here, to avoid collisions between the beta variable and std::beta in c++17
using std::cin;
using std::cout;
using std::endl;
using namespace cv;


int main( int argc, char** argv )
{


    Mat Imag;
    cout << "Opening..."<<endl;
    VideoCapture capture(0);

    for(;;)
    {
    capture >> Imag;
    Mat image = Imag;

    if( image.empty() )
    {
      cout << "Could not open or find the image!\n" << endl;
      cout << "Usage: " << argv[0] << " <Input image>" << endl;
      return -1;
    }
    Mat new_image = Mat::zeros( image.size(), image.type() );
    double alpha = 2.0; /*< Simple contrast control */
    int beta = 55;       /*< Simple brightness control */
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                new_image.at<Vec3b>(y,x)[c] =
                  saturate_cast<uchar>( alpha*image.at<Vec3b>(y,x)[c] + beta );
            }
        }
    }
    imshow("Original Image", image);
    imshow("New Image", new_image);
    int key=waitKey(1);
    if (key==27)
        break;
    }
    return 0;
}