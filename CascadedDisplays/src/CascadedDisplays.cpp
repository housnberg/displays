/**
 * Exercise 2.1 / 2.2
 * Authors: Tin Stribor Sohn
 * 			Henri Wadas
 * 			Eugen Ljavin
 */

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

const String PATH = "./";

const uint WRRI_STOPPING_CONDITION = 999;

Mat simulateCascadedDisplays(Mat imgLayerA, Mat imgLayerB) {
	int targetWidth = imgLayerA.size().width * 2;
	int targetHeight = imgLayerA.size().height * 2;

	resize(imgLayerA, imgLayerA, Size(targetWidth, targetHeight), 0, 0, INTER_NEAREST);
	resize(imgLayerB, imgLayerB, Size(targetWidth, targetHeight), 0, 0, INTER_NEAREST);

	copyMakeBorder(imgLayerA, imgLayerA, 0, 1, 0, 1, BORDER_CONSTANT);
	copyMakeBorder(imgLayerB, imgLayerB, 1, 0, 1, 0, BORDER_CONSTANT);

	Mat result = imgLayerA.mul(imgLayerB / 255.0);

	return result(Rect(1, 1, result.size().width - 2, result.size().height - 2));
}

void fixIlumination(Mat &img) {
	for (int row = 0; row < img.size().height; row++) {
		for (int col = 0; col < img.size().width; col++) {
			for (int i = 0; i < 3; i++) {
				if (img.at<Vec3f>(row, col)[i] < 0) {
					img.at<Vec3f>(row, col)[i] = 0;
				} else if (img.at<Vec3f>(row, col)[i] > 1) {
					img.at<Vec3f>(row, col)[i] = 1;
				}
			}
		}
	}
}

Mat updateColumn(Mat target, Mat input, Mat residue, bool transposed) {
	Mat numerator = Mat::zeros(target.size().height, 1, CV_32FC3);
	Mat denominator = Mat::zeros(target.size().height, 1, CV_32FC3);
	int width = residue.size().width / 2;
	for (int row = 0; row < residue.size().height - 1; row++) {
		for (int col = 0; col < residue.size().width - 1; col++) {
			int xA;
			int yA;
			int xB;
			int yB;

			if (col % 2 == 0) {
				xA = col / 2;
				xB = xA;
			} else {
				xA = (col + 1) / 2;
				xB = (col - 1) / 2;
			}
			if (row % 2 == 0) {
				yA = row / 2;
				yB = yA;
			} else {
				yA = (row + 1) / 2;
				yB = (row - 1) / 2;
			}

			int indexA = yA * width + xA;
			int indexB = yB * width + xB;

			if (transposed) {
				int tmp = indexA;
				indexA = indexB;
				indexB = tmp;
			}
			numerator.at<Vec3f>(indexA, 0) += residue.at<Vec3f>(row, col).mul(input.at<Vec3f>(indexB, 0));
			denominator.at<Vec3f>(indexA, 0) += input.at<Vec3f>(indexB, 0).mul(input.at<Vec3f>(indexB, 0));
		}
	}
	numerator = numerator / denominator;
	fixIlumination(numerator);

	return numerator;
}

void applyWRRI(Mat residue, int maxIterations, Mat &outputLayerA, Mat &outputLayerB) {
	RNG rng;
	residue.convertTo(residue, CV_32FC3, 1 / 255.0);

	int width = residue.size().width / 2;
	int height = residue.size().height / 2;
	outputLayerA = Mat(width * height, 1, CV_32FC3);
	outputLayerB = Mat(width * height, 1, CV_32FC3);

	rng.fill(outputLayerA, RNG::UNIFORM, 0.0, 1.1);
	rng.fill(outputLayerB, RNG::UNIFORM, 0.0, 1.1);
	for (int i = 0; i <= maxIterations; i++) {
		outputLayerA = updateColumn(outputLayerA, outputLayerB, residue, false);
		outputLayerB = updateColumn(outputLayerB, outputLayerA, residue, true);
	}

	outputLayerA = outputLayerA.reshape(3, height);
	outputLayerB = outputLayerB.reshape(3, height);
}

int main(int argc, char** argv) {

	Mat imgLayerA;
	Mat imgLayerB;
	Mat imgHighRes;

	imgLayerA = imread(argv[1], 1);
	imgLayerB = imread(argv[2], 1);
	imgHighRes = imread(argv[3], 1);

	if (argc < 4 || !imgLayerA.data || !imgLayerB.data || !imgHighRes.data) {
		printf ("There is one image missing! \n");
		return -1;
	}

	//Use fourth arg as path to store the images. If no arg is available, use the root as path.
	String path = PATH;
	if (argc == 5 && argv[4] != NULL) {
		path = argv[4];
	}

	namedWindow("Image Layer A", CV_WINDOW_AUTOSIZE);
	imshow("Image Layer A", imgLayerA);
	namedWindow("Image Layer B", CV_WINDOW_AUTOSIZE);
	imshow("Image Layer B", imgLayerB);
	namedWindow("High Resolution Image", CV_WINDOW_AUTOSIZE);
	imshow("High Resolution Image", imgHighRes);

	Mat result = simulateCascadedDisplays(imgLayerA, imgLayerB);
	namedWindow("Generated High Resolution Image", CV_WINDOW_AUTOSIZE);
	imshow("Generated High Resolution Image", result);
	imwrite(path + "Generated_HR_Image.png", result);

	Mat imgLowResLayerA;
	Mat imgLowResLayerB;
	applyWRRI(imgHighRes, WRRI_STOPPING_CONDITION, imgLowResLayerA, imgLowResLayerB);

	namedWindow("Generated Low Resolution Image - Layer A", CV_WINDOW_AUTOSIZE);
	imshow("Generated Low Resolution Image - Layer A", imgLowResLayerA);
	namedWindow("Generated Low Resolution Image - Layer B", CV_WINDOW_AUTOSIZE);
	imshow("Generated Low Resolution Image - Layer B", imgLowResLayerB);
	imgLowResLayerA.convertTo(imgLowResLayerA, CV_32FC3, 255.0);
	imgLowResLayerB.convertTo(imgLowResLayerB, CV_32FC3, 255.0);
	imwrite(path + "Generated_LR_Image_LayerA.png", imgLowResLayerA);
	imwrite(path + "Generated_LR_Image_LayerB.png", imgLowResLayerB);

	waitKey(0);

	return 0;
}
