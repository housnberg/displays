/**
 * Exercise 3.1 / 2.2
 * Authors: Tin Stribor Sohn
 * 			Henri Wadas
 * 			Eugen Ljavin
 */

#include "GrayCodeUtil.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

// Helper function to flip the bit
char GrayCodeUtil::flipChar(char c) {
	return (c == '0') ? '1' : '0';
}

// function to convert gray code string
// to binary string
string GrayCodeUtil::graytoBinary(string gray) {
    string binary = "";

    // MSB of binary code is same as gray code
    binary += gray[0];

    // Compute remaining bits
    for (int i = 1; i < gray.length(); i++) {
        // If current bit is 0, concatenate
        // previous bit
        if (gray[i] == '0') {
        	binary += binary[i - 1];
        }

        // Else, concatenate invert of
        // previous bit
        else {
            binary += flipChar(binary[i - 1]);
        }
    }

    return binary;
}

long GrayCodeUtil::binaryToDecimal(string binary) {
 	unsigned long long value = std::stoull(binary, 0, 2);
    return value;
}

Mat GrayCodeUtil::decode(vector<Mat> capturedGraycodes) {
	if (!capturedGraycodes.empty())	{
		Size size = capturedGraycodes[0].size();
		Mat decoded(size.width, size.height, CV_32FC1);

		for (int row = 0; row < size.height; row++) {
			for (int col = 0; col < size.width; col++) {
				string code = "";
				for (Mat graycode : capturedGraycodes) {
					code += to_string((int) graycode.at<float>(row, col));
				}
				decoded.at<float>(row, col) = binaryToDecimal(graytoBinary(code));
			}
		}

		return decoded;
	}
}

vector<Mat> GrayCodeUtil::generateGrayCode(int n) {
	int width = pow(2, n);
	int widthTmp = width;

	vector<Mat> graycodes;

    for (int bit = 0; bit < n; bit++) {
    	Mat code = Mat::zeros(1, widthTmp, CV_32FC1);
    	for (int i = 0; i < widthTmp / 2; i++) {
			code.at<float>(0, i) = 1;
		}
    	Mat codeTmp;
		for (int i = 0; i < bit; i++) {
			code.copyTo(codeTmp);
			flip(codeTmp, codeTmp, 1);
			hconcat(code, codeTmp, code);
		}
		widthTmp = widthTmp / 2;
		graycodes.push_back(code);
    }

    return graycodes;
}

vector<Mat> GrayCodeUtil::generateVerticalGraycodes(vector<Mat> graycodes, int width, int height) {
	vector<Mat> verticalGraycodes;

	string name = "+";
	for (Mat graycode : graycodes) {
		resize(graycode, graycode, Size(width, height), 0, 0, INTER_NEAREST);
		verticalGraycodes.push_back(graycode);

//		namedWindow(name, CV_WINDOW_AUTOSIZE);
//		imshow(name, graycode);
//		name += "_";
	}

	return verticalGraycodes;
}

vector<Mat> GrayCodeUtil::generateHorizontalGraycodes(vector<Mat> graycodes, int width, int height) {
	vector<Mat> horizontalGraycodes;

	string name = "_";
	for (Mat graycode : graycodes) {
		graycode = graycode.t();
		resize(graycode, graycode, Size(width, width), 0, 0, INTER_NEAREST);
		int offset = (width - height) / 2;
		graycode = graycode(Rect(0, offset, width, height));
		horizontalGraycodes.push_back(graycode);

//		namedWindow(name, CV_WINDOW_AUTOSIZE);
//		imshow(name, graycode);
//		name += ")_";
	}

	return horizontalGraycodes;
}
