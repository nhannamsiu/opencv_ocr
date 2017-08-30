#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
    Mat input = cv::imread("./images/overlay.PNG");
    Mat input_bgra;
    cvtColor(input, input_bgra, CV_BGR2BGRA);

    // find all white pixel and set alpha value to zero:
    for (int y = 0; y < input_bgra.rows; ++y)
    for (int x = 0; x < input_bgra.cols; ++x)
    {
        cv::Vec4b & pixel = input_bgra.at<cv::Vec4b>(y, x);
        // if pixel is white
        if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0)
        {
            // set alpha to zero:
            pixel[3] = 0;
        }
    }
    namedWindow("ecec", 1);
    imshow("ecec",input_bgra);
    imwrite("./images/overlay_trans.PNG", input_bgra);
    waitKey(0);
    return 0;

}
