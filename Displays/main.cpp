//
//  main.cpp
//  Displays
//


#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Function to move image along an arbitary trajectory
void translateImg(Mat &img, int offsetx, int offsety){
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,trans_mat,img.size());
}

// Define screen size
const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 1000;

int main(int argc, const char * argv[]) {
    
    //Init Screen Window
    namedWindow("Display", WINDOW_FREERATIO);
    //setWindowProperty("Display", CV_WND_PROP_FULLSCREEN,  WINDOW_FULLSCREEN);
   
    // Load image from source
    Mat source_image;
    source_image = imread("/Users/tinstriborsohn/Desktop/Uni/Erstes Semester/Analyse und Design Eingebetteter Systeme/Aufgaben/3/Displays/original_images/hair.png", 1);
    
    // Resize image to half the resolution
    Size size(900, 900);
    Mat destination_image;
    resize(source_image, destination_image, size);
    
    //Init display matrix (black)
    Mat display = Mat::zeros(SCREEN_HEIGHT, SCREEN_WIDTH, source_image.type());

    //Position destination image in the display matrix
    destination_image.copyTo(display(cv::Rect(0,0,destination_image.cols, destination_image.rows)));
    imshow("Display", display);

    //Transform destination image diagonally along screen
    while (true) {
        imshow("Display", display);
        waitKey(1);
        //update image translation
        translateImg(display, 1, 1);
    }
    
    waitKey(0);
    return 0;
}


