/*
 * GrayCodeUtil.hpp
 *
 *  Created on: Nov 15, 2018
 *      Author: el
 */

#ifndef SRC_GRAYCODEUTIL_HPP_
#define SRC_GRAYCODEUTIL_HPP_

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

class GrayCodeUtil {
public:
	GrayCodeUtil();
	static Mat decode(vector<Mat> capturedGraycodes);
	static vector<Mat> generateGrayCode(int n);
	static vector<Mat> generateVerticalGraycodes(vector<Mat> graycodes, int width, int height);
	static vector<Mat> generateHorizontalGraycodes(vector<Mat> graycodes, int width, int height);

private:
	static char flipChar(char c);
	static string graytoBinary(string gray);
	static long binaryToDecimal(string binary);

};

#endif /* SRC_GRAYCODEUTIL_HPP_ */
