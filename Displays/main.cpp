//
//  main.cpp
//  Displays Project, Resolution Enhancement through Temporal Fusion
//
// Shadi Skaff, Eugen Ljavin, Tin Stribor Sohn, Henri Wadas

#include <opencv2/opencv.hpp>


#include <iostream>
#include <math.h>

// Include Eigen Library as OpenCV does not support SparseMatrix Linear Solvers
#include <Eigen/Dense>
#include <Eigen/OrderingMethods>
#include <Eigen/IterativeLinearSolvers>
#include <opencv2/core/eigen.hpp>


using namespace cv;
using namespace std;
using namespace Eigen;


// ================================= HELPER FUNCTIONS ===================================
// Function to move Image along an arbitary Trajectory
void translateImg(Mat &img, int offsetx, int offsety){
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,trans_mat,img.size());
}

void enumerateNonZeroElements(SparseMatrix<float> mat) {
    for (int k=0; k<mat.outerSize(); ++k)
        for (SparseMatrix<float>::InnerIterator it(mat,k); it; ++it)
        {
           
             cout <<  it.value() << endl;
        }
}

// =================================== MAIN ALGORITHM ====================================

// Define Constants //

// Display Resolution
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;


// Image parameters
const string PATH = "/Users/tinstriborsohn/Desktop/Uni/Erstes Semester/Analyse und Design Eingebetteter Systeme/Aufgaben/3/Displays/original_images/";

const int SUBIMAGE_CNT = 3;
const int IMAGE_RES =  1800;

const int X_TRANSFORM = 1;
const int Y_TRANSFORM = 1;



SparseMatrix<float> calculateW(SparseMatrix<float> W) {
    
    // Calculate Retinal Integration Model
    // EQUATION: SUM X(p(t))Xk
    // if p(t) is in pixel i, j => X(p(t)) = 1 else X(p(t)) = 0
    // For each pixel calculate path(p(t)) through all other pixels ( subimage count values in each row )
    
    // Improve write performance by reserving space with a Triplet List
    typedef Triplet <float> T;
    vector<T> tripletList;
    tripletList.reserve(W.cols()*SUBIMAGE_CNT);

    for (int i = 0; i < W.cols(); i++) {
        
        for (int j = 0; j < SUBIMAGE_CNT; j++) {
            
            if (j == 0) {
                
                // p(t) starts from the observed pixel
                //W.insert(i, i) = 1.0f;
                tripletList.push_back(T(i,i,1.0f));
                
            } else {
                
                // p(t) movement through all subsequent pixels for each timestep of the interval
                int next_el = j*(i+IMAGE_RES-1+Y_TRANSFORM+X_TRANSFORM);
                
                if ( next_el < W.rows() && next_el > 0) {
                     //W.insert(i, next_el) = 1.0f;
                     tripletList.push_back(T(i,next_el,1.0f));
                }
                
            }
            
        }
    }
    
    W.setFromTriplets(tripletList.begin(), tripletList.end());
    
    cout << "Calculation of W based on transform DONE" << endl;
    
    return W;
    
}

MatrixXf calculateILWithLeastSquares(MatrixXf IL,  MatrixXf IH, SparseMatrix<float> W) {
    
    // Solving linear least squares problem with sparse and overdetermined matrices (more values in W then variables in IL cf. paper)
    LeastSquaresConjugateGradient<SparseMatrix<float>> ls;
    ls.compute(W);
    IL= ls.solve(IH);
    
    cout << "Calculation of IL With Least Squares Calculation DONE" << endl;
    
    return IL;
}

MatrixXf convertMatToEigen(Mat mat, MatrixXf eigen) {
    
    for (int i = 0; i < mat.rows; i++) {
        for (int j= 0; j < mat.cols; j++) {
            eigen(i, j) = (float)mat.at<uchar>(i,j);
        }
    }
    return eigen;
}

int main(int argc, const char * argv[]) {
    
    // Init Display Screen Window
    namedWindow("Display", WINDOW_FULLSCREEN);
    
    // Load high resolution image from source to OpenCV Matrix IH
    Mat IH;
    string image_name = "dragon.png";
    IH = imread(PATH+image_name, 1);
    cout << (float)IH.at<uchar>(13, 23) << endl;
    
    // Create Column Vector out of destination image
    Mat targetIH = IH.reshape(0, 1);
    targetIH = targetIH.t();
    cout << targetIH.rows << endl;
     cout << targetIH.cols << endl;
    
    
    // Map target IH data to Eigen vector
    MatrixXf IH_Eigen(targetIH.rows,  targetIH.cols);
    IH_Eigen = convertMatToEigen(targetIH, IH_Eigen);
    cout << IH_Eigen.rows() << endl;
    
    // Init Eigen SparseMatrix for W ( Weights based on transform (p(t))
    SparseMatrix<float> W(IH.rows*IH.cols, IH.rows*IH.cols);
    cout << W.rows() << endl;
    
    // calculate Weights based on transform
    W = calculateW(W);
    //enumerateNonZeroElements(W);
    
    MatrixXf IL(targetIH.rows/SUBIMAGE_CNT, targetIH.cols);
    IL = calculateILWithLeastSquares(IL, IH_Eigen, W);
    cout << IL << endl;
    cout <<  IL.rows() << endl;
    
    
    
    imshow("Display", IH);
    
    
    //LOOP
    
    // Render the n Subimages on the screen
    
    // Shift the whole thing by one pixel
    
    
    
    
    // Resize image to half the resolution
    //Size size(IMAGE_SCALING, IMAGE_SCALING);
    //Mat destination_image;
    //resize(IH, destination_image, size);
    
    // Init display matrix (black)
    /*Mat display = Mat::zeros(SCREEN_HEIGHT, SCREEN_WIDTH, IH.type());
     
     
     // DO THE STUFF
     
     
     // Position subimages in the display matrix
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
     */
    
    
    
    waitKey(0);
    return 0;
}


