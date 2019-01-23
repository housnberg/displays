//
//  main.cpp
//  Displays Project, Resolution Enhancement through Temporal Fusion
//
// Shadi Skaff, Eugen Ljavin, Tin Stribor Sohn, Henri Wadas

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// ================================= HELPER FUNCTIONS ===================================
// Function to move Image along an arbitary Trajectory
void translateImg(Mat &img, int offsetx, int offsety){
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,trans_mat,img.size());
}

// Function to calculate integral
double integral(double(*f)(double x), double a, double b, int n) {
    double step = (b - a) / n;
    double area = 0.0;
    for (int i = 0; i < n; i ++) {
        area += f(a + (i + 0.5) * step) * step;
    }
    return area;
}

// =================================== MAIN ALGORITHM ====================================

// Define Constants

// Display Resolution
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 1800;

// Image parameters
const string PATH = "/Users/tinstriborsohn/Desktop/Uni/Erstes Semester/Analyse und Design Eingebetteter Systeme/Aufgaben/3/Displays/original_images/";
const int IMAGE_SCALING = SCREEN_WIDTH/2;
const int SUBIMAGE_CNT = 3;
const int X_TRANSFORM = 1;
const int Y_TRANSFORM = 1;


Mat getWeightingMatrix(Mat IH, int transformX, int transformY) {
    
    // Calculate Retinal Integration Model
    
    // Init Weighting Matrix with Zeros
    Mat W = Mat::zeros(SCREEN_WIDTH, SCREEN_HEIGHT, IH.type());;
    
    // Integration model
    // BASE EQUATION: w(x,y,t) = INTEGRAL (0toT) { X(i, j)(p(t)) X(t) } dt

    // iterate through interval
    for (int t = 0; t < SUBIMAGE_CNT; t++) {
        
        // iterate through High Res Image Rows and cols
        for (int i = 0; i < IH.cols; i++) {
            for (int j = 0; j < IH.rows; j++) {
                
                // if pixel(i, j) lies in transition path p(t) = Xpt is 1 else 0
                cout << IH.at<uchar>(i, j) << endl;
            }
        }
        
    }
   
    
    
    return W;
    
}

Mat calculateILWithleastSquares(Mat IH, int transformX, int transformY) {
    
    // BASE EQUATION :  W * I°L = I°H
    // Least Squares Criterion: I°L = (W'W)°-1*W'I°H
    
    // Define Weighting Matrix (Retinal Integration)
    //Mat WeightingMatrix = getWeightingMatrix(IH, transformX, transformY);
    //Mat WeightingMatrixTransposed;
    //transpose(WeightingMatrix, WeightingMatrixTransposed);
    
    // Calculate Least Squares
    //Mat IL = (WeightingMatrix*WeightingMatrixTransposed).inv() * WeightingMatrixTransposed * IH;
    
    return IH;
    
}

void stackSubimages() {
    
}

void reduceFlickering() {
    
}

int main(int argc, const char * argv[]) {
    
    // Init Screen Window
    namedWindow("Display", WINDOW_FREERATIO);
    //setWindowProperty("Display", CV_WND_PROP_FULLSCREEN,  WINDOW_FULLSCREEN);
   
    // Load high resolution image from source IH
    Mat IH;
    string image_name = "hair.png";
    IH = imread(PATH+image_name, 1);
    
   
    // Do Calculations of Subimages
    
    
    
    //LOOP
    
    // Render the Subimages on the screen
    
    // Shift the whole thing by one pixel
    
    
    
    
    // Resize image to half the resolution
    Size size(IMAGE_SCALING, IMAGE_SCALING);
    Mat destination_image;
    resize(IH, destination_image, size);
    
    // Init display matrix (black)
    Mat display = Mat::zeros(SCREEN_HEIGHT, SCREEN_WIDTH, IH.type());
    
  
    // DO THE STUFF
    
    
    // Position destination image in the display matrix
    destination_image.copyTo(display(cv::Rect(0,0,destination_image.cols, destination_image.rows)));
    imshow("Display", display);

    cout << display.size() << endl;

    
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
            currentXPos -= X_TRANSFORM;
            currentYPos -= Y_TRANSFORM;
            translateImg(display, -X_TRANSFORM, -Y_TRANSFORM);
        } else {
            currentXPos += X_TRANSFORM;
            currentYPos += Y_TRANSFORM;
            translateImg(display, X_TRANSFORM, Y_TRANSFORM);
        }
        
        
        if (currentXPos == SCREEN_WIDTH - IMAGE_SCALING ) {
            backward = true;
        } else if (currentXPos == 0){
            backward = false;
        }
    }
    
    waitKey(0);
    return 0;
}


