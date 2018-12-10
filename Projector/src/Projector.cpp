/**
 * Exercise 3.1 / 3.2 / 3.3
 * Authors: Tin Stribor Sohn
 * 			Henri Wadas
 * 			Eugen Ljavin
 */

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>
#include "GrayCodeUtil.hpp"
#include "PGCapture.hpp"

using namespace cv;
using namespace std;

const int PROJECTOR_WIDTH = 1024;
const int PROJECTOR_HEIGHT = 768;

PGCapture* cam;

vector<Mat> projectAndCaptureImgs(vector<Mat> imgs, string filePrefix, int showTime = 500) {
	vector<Mat> capturedImgs;

	for (int i = 0; i < (int) imgs.size(); i++) {
		imshow("Pattern", imgs[i]);
		waitKey(showTime);
		Mat capturedImg = cam->getImage();
		cvtColor(capturedImg, capturedImg, COLOR_RGB2GRAY);
		String imgName = "./" + filePrefix + "-img_" + std::to_string(i) + ".png";
		capturedImgs.push_back(capturedImg);
		imwrite(imgName, capturedImg);
		waitKey(showTime);
	}

	return capturedImgs;
}

//see https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/laplace_operator/laplace_operator.html
Mat calcLaplacian(Mat coloredImg, bool applyGaussianBlur = true) {
	int ddepth = CV_16S;
	int kernelSize = 3;
	//int kernelSize = 5;
	int scale = 1;
	int delta = 0;

	Mat target;

	if (applyGaussianBlur) {
		GaussianBlur(coloredImg, coloredImg, Size(3, 3), 0, 0, BORDER_DEFAULT);
	}
	cvtColor(coloredImg, coloredImg, COLOR_RGB2GRAY);

	Laplacian(coloredImg, target, ddepth, kernelSize, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(target, target);

	return target;
}

int main(int argc, char** argv) {

	cam = new PGCapture();

	int bits = ceil(log2(PROJECTOR_WIDTH));

	vector<Mat> graycodes = GrayCodeUtil::generateGrayCode(bits);
	vector<Mat> columnGraycodes = GrayCodeUtil::encode(graycodes, PROJECTOR_WIDTH, PROJECTOR_HEIGHT);
	vector<Mat> rowGraycodes = GrayCodeUtil::encode(graycodes, PROJECTOR_WIDTH, PROJECTOR_HEIGHT, false);

//	Mat decodedColumns = GrayCodeUtil::decode(columnGraycodes);
//	Mat decodedRows = GrayCodeUtil::decode(rowGraycodes, false);

	Mat capturedOriginalImg = cam->getImage();
	Mat originalImgAsLaplacian = calcLaplacian(capturedOriginalImg);
	imwrite("./original-img.png", capturedOriginalImg);
	imwrite("./original-img_laplacian.png", originalImgAsLaplacian);

	vector<Mat> capturedColumnImgs = projectAndCaptureImgs(columnGraycodes, "column");
	vector<Mat> capturedRowImgs = projectAndCaptureImgs(rowGraycodes, "row");


	waitKey(0);

	return 0;
}
