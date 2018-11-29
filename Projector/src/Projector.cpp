/**
 * Exercise 2.1 / 2.2
 * Authors: Tin Stribor Sohn
 * 			Henri Wadas
 * 			Eugen Ljavin
 */

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>
#include "GrayCodeUtil.hpp"

using namespace cv;
using namespace std;

void generateGrayCodeImages(uint width, uint height) {
	int bits = ceil(log2(width));

	vector<Mat> graycodes = GrayCodeUtil::generateGrayCode(bits);
	vector<Mat> horizontalGraycodes = GrayCodeUtil::generateHorizontalGraycodes(graycodes, width, height);
	vector<Mat> verticalGraycodes = GrayCodeUtil::generateVerticalGraycodes(graycodes, width, height);

	Mat decodedColumns = GrayCodeUtil::decode(verticalGraycodes);

//	for (int row = 0; row < decodedColumns.size().height; row++) {
//		for (int col = 0; col < decodedColumns.size().width; col++) {
//			cout << "_" << decodedColumns.at<float>(row, col) << "_"  << endl;
//		}
//	}
}

int main(int argc, char** argv) {

	generateGrayCodeImages(1024, 768);

	waitKey(0);

	return 0;
}
