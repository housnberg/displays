//
//  Exercise 02.01 Code
//  Displays WS18/19
//  Ljavin Eugen, Wadas Henri, Sohn Tin Stribor
//

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

const String PATH = "./img/";

Mat cascade(Mat layer_a, Mat layer_b) {
    
    //Nearest neighbours method for resizing images
    resize(layer_a, layer_a, Size(layer_a.size().width * 2, layer_a.size().height * 2), 0, 0, INTER_NEAREST);
    resize(layer_b, layer_b, Size(layer_a.size().width * 2, layer_a.size().height * 2), 0, 0, INTER_NEAREST);
    
    copyMakeBorder(layer_a, layer_a, 0, 1, 0, 1, BORDER_CONSTANT);
    copyMakeBorder(layer_b, layer_b, 1, 0, 1, 0, BORDER_CONSTANT);
    
    Mat result = layer_a.mul(layer_b / 255.0);
    
    return result(Rect(1, 1, result.size().width - 2, result.size().height - 2));
}

void correctIlluminance(Mat &img) {
    for (int row = 0; row < img.size().height; row++) {
        for (int col = 0; col < img.size().width; col++) {
            for (int i = 0; i < 3; i++) {
                //go through 3 channels
                if (img.at<Vec3f>(row, col)[i] < 0) {
                    img.at<Vec3f>(row, col)[i] = 0;
                } else if (img.at<Vec3f>(row, col)[i] > 1) {
                    img.at<Vec3f>(row, col)[i] = 1;
                }
            }
        }
    }
}


Mat factorize(Mat target, Mat input, Mat residue, bool T) {
    Mat num1 = Mat::zeros(target.size().height, 1, CV_32FC3);
    Mat num2 = Mat::zeros(target.size().height, 1, CV_32FC3);
    
    for (int row = 0; row < residue.size().height - 1; row++) {
        for (int col = 0; col < residue.size().width - 1; col++) {
            
            //x an y coordinates of y and b
            int x_a, y_a, x_b, y_b;
            
            // shift half a pixel right on x and down y axis
            if (row % 2 == 0) {
                y_a = row / 2;
                y_b = y_a;
            } else {
                y_a = (row + 1) / 2;
                y_b = (row - 1) / 2;
            }
            
            if (col % 2 == 0) {
                x_a = col / 2;
                x_b = x_a;
            } else {
                x_a = (col + 1) / 2;
                x_b = (col - 1) / 2;
            }
            
            //factorization vals
            int a_i = y_a * residue.size().width + x_a;
            int b_i = y_b * residue.size().width + x_b;
            
            //check if transponed (param)
            if (T) {
                //swap elements
                int current = a_i;
                a_i = b_i;
                b_i = current;
            }
            
            //Elementwise multiplication
            num1.at<Vec3f>(a_i, 0) += residue.at<Vec3f>(row, col).mul(input.at<Vec3f>(b_i, 0));
            num2.at<Vec3f>(a_i, 0) += input.at<Vec3f>(b_i, 0).mul(input.at<Vec3f>(b_i, 0));
        }
    }
    
    num1 = num1 / num2;
    correctIlluminance(num1);
    
    return num1;
}


void wrri(Mat residue, int iterations, Mat &out_a, Mat &out_b) {
    RNG rng;
    residue.convertTo(residue, CV_32FC3, 1 / 255.0);
    
    int width = residue.size().width / 2;
    int height = residue.size().height / 2;
    out_a = Mat(width * height, 1, CV_32FC3);
    out_b = Mat(width * height, 1, CV_32FC3);
    
    rng.fill(out_a, RNG::UNIFORM, 0.0, 1.1);
    rng.fill(out_b, RNG::UNIFORM, 0.0, 1.1);
    
    for (int i = 0; i <= iterations; i++) {
        out_a = factorize(out_a, out_b, residue, false);
        out_b = factorize(out_b, out_a, residue, true);
    }
    
    out_a = out_a.reshape(3, height);
    out_b = out_b.reshape(3, height);
}


int main(int argc, char** argv) {
    
    Mat layer_a;
    Mat layer_b;
    Mat target;
    
    //read image data
    layer_a = imread(argv[1], 1);
    layer_b = imread(argv[2], 1);
    target = imread(argv[3], 1);
    
    if (argc < 4 || !layer_a.data || !layer_b.data || !target.data) {
        printf ("Not enough args / No image data \n");
        return -1;
    }
    
    
    namedWindow("Layer A", CV_WINDOW_AUTOSIZE);
    imshow("Layer A", layer_a);
    namedWindow("Layer B", CV_WINDOW_AUTOSIZE);
    imshow("Layer B", layer_b);
    namedWindow("High Res", CV_WINDOW_AUTOSIZE);
    imshow("High Res", target);
    
    Mat result = cascade(layer_a, layer_b);
    namedWindow("Cascaded High Res", CV_WINDOW_AUTOSIZE);
    imshow("Cascaded High Res", result);
    imwrite(PATH + "Cascaded_HighRes_Image.png", result);
    
    Mat wrri_a;
    Mat wrri_b;
    wrri(target, 999, wrri_a, wrri_b);
    
    namedWindow("WRRI - Layer A", CV_WINDOW_AUTOSIZE);
    imshow("WRRI - Layer A", wrri_a);
    namedWindow("WRRI - Layer B", CV_WINDOW_AUTOSIZE);
    imshow("WRRI - Layer B", wrri_b);
    
    
    wrri_a.convertTo(wrri_a, CV_32FC3, 255.0);
    wrri_b.convertTo(wrri_b, CV_32FC3, 255.0);
    imwrite(PATH + "wrri_a.png", wrri_a);
    imwrite(PATH + "wrri_b.png", wrri_b);
    
    waitKey(0);
    
    return 0;
}




