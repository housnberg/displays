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

const float THRESHOLD = 0.075;

//function from https://www.geeksforgeeks.org/gray-to-binary-and-binary-to-gray-conversion/
string GrayCodeUtil::grayToBinary(string gray) {
    string binary = "";
    binary += gray[0];

    for (int i = 1; i < gray.length(); i++) {
        if (gray[i] == '0') {
        	binary += binary[i - 1];
        } else {
        	binary += (binary[i - 1] == '0') ? '1' : '0';
        }
    }

    return binary;
}

long GrayCodeUtil::binaryToDecimal(string binary) {
 	unsigned long long value = std::stoull(binary, 0, 2);
    return value;
}

Mat GrayCodeUtil::decode(vector<Mat> capturedGraycodes, bool columns) {
	if (!capturedGraycodes.empty())	{
		Size size = capturedGraycodes[0].size();
		Mat decoded(size.height, size.width, CV_32FC1);
		int offset = (size.width - size.height) / 2;

		for (int row = 0; row < size.height; row++) {
			for (int col = 0; col < size.width; col++) {
				string code = "";
				for (Mat graycode : capturedGraycodes) {
					float value = graycode.at<float>(row, col);
					//todo threshold
					if (1 - THRESHOLD <= value) {
						code += "1";
					}
					if (0 + THRESHOLD >= value) {
						code += "0";
					}
//					code += to_string((int) value);
				}
				float decodedValue = binaryToDecimal(grayToBinary(code));
				if (!columns) {
					decodedValue -= offset;
				}
				decoded.at<float>(row, col) = decodedValue;
//				cout << decoded.at<float>(row, col) << endl;
			}
		}

		return decoded;
	}
}

vector<Mat> GrayCodeUtil::generateGrayCode(int n, bool inverted) {
	int width = pow(2, n);
	int widthTmp = width;
	int value = 0;

	vector<Mat> graycodes;

    for (int bit = 0; bit < n; bit++) {
    	Mat code = Mat::ones(1, widthTmp, CV_32FC1);
    	if (inverted) {
    		code = Mat::zeros(1, widthTmp, CV_32FC1);
    		value = 1;
    	}
    	for (int i = 0; i < widthTmp / 2; i++) {
			code.at<float>(0, i) = value;
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


vector<Mat> GrayCodeUtil::encode(vector<Mat> graycodes, int width, int height, bool columns) {
	vector<Mat> encodedGraycodes;

	for (Mat graycode : graycodes) {
		if (columns) {
			resize(graycode, graycode, Size(width, height), 0, 0, INTER_NEAREST);
		} else {
			graycode = graycode.t();
			resize(graycode, graycode, Size(width, width), 0, 0, INTER_NEAREST);
			int offset = (width - height) / 2;
			graycode = graycode(Rect(0, offset, width, height));
		}
		encodedGraycodes.push_back(graycode);
	}

	return encodedGraycodes;
}

