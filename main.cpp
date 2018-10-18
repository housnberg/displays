//
//  Exercise 01 Code
//  Displays WS18/19
//


#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>

using namespace cv;

int main( int argc, char** argv ) {
    
    Mat image;
    Mat corrected;
    Mat difference;
    
    Mat correctGamma ( Mat img, double gamma);
    Mat calculateDifference ( Mat img, Mat corrected );
    
    // Set the writePath for storing the manipulated images
    String writePath = "./";
    
    // Read default image data from argument
    image = imread( argv[1], 1 );
    if( argc != 2 || !image.data ) {
        printf( "No image data \n" );
        return -1; }
    
    // Save default image
    imwrite(writePath + "default.bmp", image);
    
   
    // Call Gamma Correct Function with gamma value 0.5 and show corrected Image on a new window
    corrected = correctGamma(image, 0.5f);
  
    // Reload default image as somehow the original Mat gets manipulated in the getter functions
    image = imread(writePath + "default.bmp", 1 );
    if( argc != 2 || !image.data ) {
        printf( "No image data \n" );
        return -1; }
    
    // save gamma corrected Image
    imwrite(writePath + "corrected.bmp", corrected);

    // Call Difference Calculation Function and show corrected image on a new window
    difference = calculateDifference(image, corrected);

    // Save difference image
    imwrite(writePath + "difference.bmp", difference);
    
    // Create windows and show images
    namedWindow("Default", CV_WINDOW_AUTOSIZE);
    imshow("Default", image);
    
    
    namedWindow("Gamma Corrected", CV_WINDOW_AUTOSIZE);
    imshow("Gamma Corrected", corrected);
    
    
    namedWindow("Difference Image", CV_WINDOW_AUTOSIZE);
    imshow("Difference Image", difference);
    
    waitKey (0);
    return 0;
    
}

Mat correctGamma ( Mat img, double gamma ) {
    
    Mat correctedImg = img;
    
    // Iterate through each pixel column (y)- and row (x)-wise
    for( int y_index = 0; y_index < img.size().height; y_index++) {
        for( int x_index = 0; x_index < img.size().width; x_index++) {
            
            // Define a 3d Vector which is equals to the 3d BGR Vector of img
            Vec3b intensity = correctedImg.at<Vec3b>(y_index, x_index);
            
            // Get BGR Values from Image BGR Vector at Pixel Index
            double blue = correctedImg.at<Vec3b>(y_index, x_index)[0];
            double green = correctedImg.at<Vec3b>(y_index, x_index)[1];
            double red = correctedImg.at<Vec3b>(y_index, x_index)[2];
            
            // Calculate gamma and assign to intensity vector
            intensity.val[0] = std::pow(blue/255, gamma)*255;
            intensity.val[1] = std::pow(green/255, gamma)*255;
            intensity.val[2] = std::pow(red/255, gamma)*255;
            
            // Replace old img BGR Values at pixel index with the gamma corrected intensity
            correctedImg.at<Vec3b>(y_index, x_index) = intensity;
            
        }
    }
    
    return correctedImg;
}

Mat calculateDifference (Mat img, Mat corrected) {
    Mat differenceImg;
  
    // Get the absolute difference of the image arrays
    absdiff(corrected, img, differenceImg);
    
    // DEBUG check if images are equal
    bool eq = countNonZero(img!=corrected) == 1;
    if (eq) {
        std::cout <<  "images are equal !";
    }
    
    return differenceImg;
}


