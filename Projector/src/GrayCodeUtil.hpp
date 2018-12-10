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
	static vector<Mat> generateGrayCode(int n, bool inverted = false);
	static vector<Mat> encode(vector<Mat> graycodes, int width, int height, bool columns = true);
	static Mat decode(vector<Mat> capturedGraycodes, bool columns = true);

private:
	static string grayToBinary(string gray);
	static long binaryToDecimal(string binary);

};

#endif /* SRC_GRAYCODEUTIL_HPP_ */
