//
//  main.cpp
//  Displays
//w


#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Function to move Image along an arbitary Trajectory
void translateImg(Mat &img, int offsetx, int offsety){
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,trans_mat,img.size());
}

// Define Screen Size
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int IMAGE_SCALING = SCREEN_WIDTH/2;

Mat getWeightingMatrix(Mat IH, int transformX, int transformY) {
    
    // Calculate Retinal Integration Model
    Mat W;
    
    return W;
    
}

Mat calculateILWithleastSquares(Mat IH, int transformX, int transformY) {
    
    // BASE EQUATION :  W * I°L = I°H
    // Least Squares Criterion: I°L = (W'W)°-1*W'I°H
    
    // Define Weighting Matrix (Retinal Integration)
    Mat WeightingMatrix = getWeightingMatrix(IH, transformX, transformY);
    Mat WeightingMatrixTransposed;
    transpose(WeightingMatrix, WeightingMatrixTransposed);
    
    // Calculate Least Squares
    Mat IL = (WeightingMatrix*WeightingMatrixTransposed).inv() * WeightingMatrixTransposed * IH;
    
    return IL;
    
}

void stackSubimages() {
    
}

void reduceFlickering() {
    
}

int main(int argc, const char * argv[]) {
    
    // Init Screen Window
    namedWindow("Display", WINDOW_FREERATIO);
    //setWindowProperty("Display", CV_WND_PROP_FULLSCREEN,  WINDOW_FULLSCREEN);
   
    // Load image from source
    Mat source_image;
    string image_name = "hair.png";
    source_image = imread("/Users/tinstriborsohn/Desktop/Uni/Erstes Semester/Analyse und Design Eingebetteter Systeme/Aufgaben/3/Displays/original_images/"+image_name, 1);
    
    // Resize image to half the resolution
    Size size(IMAGE_SCALING, IMAGE_SCALING);
    Mat destination_image;
    resize(source_image, destination_image, size);
    
    // Init display matrix (black)
    Mat display = Mat::zeros(SCREEN_HEIGHT, SCREEN_WIDTH, source_image.type());

    // Position destination image in the display matrix
    destination_image.copyTo(display(cv::Rect(0,0,destination_image.cols, destination_image.rows)));
    imshow("Display", display);

    // Transform destination image diagonally along screen
    int currentXPos = 0;
    int currentYPos = 0;
    bool backward = false;
    
    // Init timer
    int t = 0;
    
    while (true) {
        
        // Increment timer
        t++;
        
        // Show screen
        imshow("Display", display);
        waitKey(1);
       
        // Move image back and forth diagonally across the screen
        if (backward == true) {
            currentXPos -= 2;
            currentYPos -= 1;
            translateImg(display, -3, -1);
        } else {
            currentXPos += 2;
            currentYPos += 1;
            translateImg(display, 3, 1);
        }
        
        
        if (currentXPos == SCREEN_WIDTH/2 - IMAGE_SCALING/2 ) {
            backward = true;
        } else if (currentXPos == 0){
            backward = false;
        }
    }
    
    waitKey(0);
    return 0;
}


