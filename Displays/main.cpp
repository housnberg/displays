//
//  main.cpp
//  Displays Project, Resolution Enhancement through Temporal Fusion
//
// Shadi Skaff, Eugen Ljavin, Tin Stribor Sohn, Henri Wadas

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;


// ================================= HELPER FUNCTIONS ===================================
// Function to move Image along an arbitary Trajectory
void translateImg(Mat &img, int offsetx, int offsety){
    Mat trans_mat = (Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img,img,trans_mat,img.size());
}

// Helper Function to calculate integral
double integral(double(*f)(double x), double a, double b, int n) {
    double step = (b - a) / n;
    double area = 0.0;
    for (int i = 0; i < n; i ++) {
        area += f(a + (i + 0.5) * step) * step;
    }
    return area;
}

// Debug function to view elements of W
void printNonZeroElementsOfW(SparseMat sparseMat) {
    
    SparseMatConstIterator_<float>
    it = sparseMat.begin<float>(),
    it_end = sparseMat.end<float>();
    double s = 0;
    int dims = sparseMat.dims();
    for(; it != it_end; ++it)
    {
        // print element indices and the element value
        const SparseMat::Node* n = it.node();
        printf("(");
        for(int i = 0; i < dims; i++)
            printf("%d%s", n->idx[i], i < dims-1 ? ", " : ")");
        printf(": %g\n", it.value<float>());
        s += *it;
    }
}

// =================================== MAIN ALGORITHM ====================================

// Define Constants

// Display Resolution
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;


// Image parameters
const string PATH = "/Users/tinstriborsohn/Desktop/Uni/Erstes Semester/Analyse und Design Eingebetteter Systeme/Aufgaben/3/Displays/original_images/";

const int SUBIMAGE_CNT = 3;

const int IMAGE_RES =  1800;

const int X_TRANSFORM = 1;
const int Y_TRANSFORM = 1;



SparseMat calculateW(SparseMat W, int transformX, int transformY) {
    
    // Calculate Retinal Integration Model
    // EQUATION: SUM X(p(t))Xk
    // if p(t) is in pixel i, j => X(p(t)) = 1 else X(p(t)) = 0
    // For each pixel calculate path(p(t)) through all other pixels ( subimage count values in each row )
    
    for (int i = 0; i < W.size(0); i++) {
        
        for (int j = 0; j < SUBIMAGE_CNT; j++) {
            
            if (j == 0) {
                
                // p(t) starts from the observed pixel
                W.ref<float>(i, i) = 1.0f;
                
            } else {
                
                // p(t) movement through all subsequent pixels for each timestep of the interval
                int next_el = j*(i+IMAGE_RES-1+Y_TRANSFORM+X_TRANSFORM);
                
                if ( next_el < W.size(1) && next_el > 0) {
                     W.ref<float>(i, next_el) = 1.0f;
                }
                
            }
            
        }
    }
    
    cout << "Calculation of W based on transform DONE" << endl;
    
    
    return W;
    
}

Mat calculateleastSquares(Mat IH, SparseMat W, Mat IL) {
    
    // BASE EQUATION :  W * I°L = I°H
    // Least Squares Criterion: I°L = (W'W)°-1*W'I°H
    
    // Define Weighting Matrix (Retinal Integration)
    //Mat WeightingMatrix = getWeightingMatrix(IH, transformX, transformY);
    //Mat WeightingMatrixTransposed;
    //transpose(WeightingMatrix, WeightingMatrixTransposed);
    
    // Calculate Least Squares
    //Mat IL = (WeightingMatrix*WeightingMatrixTransposed).inv() * WeightingMatrixTransposed * IH;
    
    return IL;
    
}

int main(int argc, const char * argv[]) {
    
    // Init DisplaynScreen Window
    namedWindow("Display", WINDOW_FULLSCREEN);
    
    // Load high resolution image from source IH
    Mat IH;
    string image_name = "hair.png";
    IH = imread(PATH+image_name, 1);
    
    // init W sparse matrix DIM: 3240000 x 3240000
    // one row is equals to one receptor
    int w_size[] = {IH.rows*IH.cols, IH.rows*IH.cols};
    SparseMat W(2, w_size, CV_32F);
    cout << W.size(1) << endl;
    
    W = calculateW(W, X_TRANSFORM, Y_TRANSFORM);
    printNonZeroElementsOfW(W);
    
    // init low res matrix ( 3 subimages ) from IH DIM: 3240000x1
    Mat IL = Mat::zeros(1, IH.rows*IH.cols, IH.type());
    cout << IL.size() << endl;
    
    // reshape IH to DIM: 3240000 x 1
    Mat targetIH = IH.reshape(0, 1);
    cout << targetIH.size() << endl;
    
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


