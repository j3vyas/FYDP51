#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <opencv2/videoio.hpp>

cv::Scalar hsv_min_red(130, 100, 100);
cv::Scalar hsv_max_red(179, 140, 140);
cv::Scalar hsv_min_blue(140, 0, 40);
cv::Scalar hsv_max_blue(179, 100, 100);
cv::Scalar hsv_min_green(50, 60, 40);
cv::Scalar hsv_max_green(80, 190, 90);
cv::Scalar hsv_min_yellow(20, 60, 160);
cv::Scalar hsv_max_yellow(40, 100, 200);
cv::Scalar bgr_red(0, 0, 255);
cv::Scalar bgr_blue(255, 0, 0);
cv::Scalar bgr_green(0, 255, 0);
cv::Scalar bgr_yellow(0, 255, 255);

void drawObjects(std::vector<cv::RotatedRect> boundingBox, std::vector<cv::Point> centrePoints, cv::Mat &drawing, cv::Scalar colour){
	// draw the rotated rect and centre point
	for (int i = 0; i < boundingBox.size(); i++){
		cv::Point2f corners[4];
		boundingBox[i].points(corners);
		cv::line(drawing, corners[0], corners[1], colour);
		cv::line(drawing, corners[1], corners[2], colour);
		cv::line(drawing, corners[2], corners[3], colour);
		cv::line(drawing, corners[3], corners[0], colour);
		cv::circle(drawing, centrePoints[i], 3, colour, -1);
	}
}

void trackObjects(cv::Mat &img_filtered, cv::Mat &drawing, cv::Scalar colour){

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(img_filtered, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//zero matrix to draw on
	drawing = cv::Mat::zeros(img_filtered.size(), CV_8UC3);

	std::vector<cv::RotatedRect> boundingBox;
	std::vector<cv::Point> centrePoints;
	for (int i = 0; i < contours.size(); i++){
		cv::RotatedRect rect = cv::minAreaRect(contours[i]);
		cv::Point2f corners[4];
		rect.points(corners);
		bool isDetected = true;
		cv::Point2f prevCorner;
		prevCorner = corners[0];

		for (int j = 1; j < 4; j++){
		if (corners[j] == prevCorner)
		isDetected = false;
		}

		if (isDetected && (cv::contourArea(contours[i]) >= 1000)){
			boundingBox.push_back(rect);
			centrePoints.push_back(rect.center);
		}
	}
	drawObjects(boundingBox, centrePoints, drawing, colour);
}

void morphImg(cv::Mat &img){
	// use erosion and dilation to remove noise
	cv::Mat erodeElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));

	cv::erode(img, img, erodeElement);
	cv::erode(img, img, erodeElement);

	cv::dilate(img, img, dilateElement);
	cv::dilate(img, img, dilateElement);
}

int main(int argc, char **argv) {

	// try capturing image from the webcam
	cv::VideoCapture cap(0);

	// exit if capture wasn't successful
	if (!cap.isOpened()){
		std::cout << "webcam not on" << std::endl;
		return -1;
	}

	cv::Mat img_orig;

	cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);
	int iLowH = 0;
	int iHighH = 0;
	int iLowS = 0;
	int iHighS = 0;
	int iLowV = 0;
	int iHighV = 255;

	cv::createTrackbar("LowH", "Control", &iLowH, 179);
	cv::createTrackbar("HighH", "Control", &iHighH, 179);

	cv::createTrackbar("LowS", "Control", &iLowS, 255);
	cv::createTrackbar("HighS", "Control", &iHighS, 255);

	cv::createTrackbar("LowV", "Control", &iLowV, 255);
	cv::createTrackbar("HighV", "Control", &iHighV, 255);

	// capture loop
	while (true){

		//break the loop if capture isn't successful
		bool success = cap.read(img_orig);
		if (!success){
			std::cout << "read unccessful" << std::endl;
			break;
		}

		// convert input image to HSV
		cv::Mat img_hsv;
		//cv::medianBlur(img_orig, img_orig, 3);
		cv::cvtColor(img_orig, img_hsv, cv::COLOR_BGR2HSV);

		// threshold the HSV image, keep only the red pixels
		// find red first
		cv::Mat threshold;
		//cv::GaussianBlur(img_orig, img_orig, cv::Size(5, 5), 2, 2);
		cv::inRange(img_hsv, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), threshold);
		morphImg(threshold);
		cv::imshow("morphed", threshold);
		cv::Mat test;
		trackObjects(threshold, test, bgr_red);

		// display images
		cv::imshow("original", img_orig);
		cv::imshow("test", test);

		if (cv::waitKey(30) == 27){
			break;
		}
	}
	return 0;
}