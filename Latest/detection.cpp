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
#include "ChairProcessor.h"
#include <thread>
#include <chrono>

cv::Scalar hsv_red_min_lower(0, 80, 20);
cv::Scalar hsv_red_max_lower(10, 120, 140);
cv::Scalar hsv_red_min_upper(160, 80, 20);
cv::Scalar hsv_red_max_upper(179, 120, 140);
cv::Scalar hsv_purple_min(110, 20, 40);
cv::Scalar hsv_purple_max(140, 70, 90);
cv::Scalar hsv_green_min(50, 60, 40);
cv::Scalar hsv_green_max(80, 190, 90);
cv::Scalar hsv_yellow_min(10, 100, 40);
cv::Scalar hsv_yellow_max(30, 255, 100);
cv::Scalar bgr_red(0, 0, 255);
cv::Scalar bgr_purple(255, 0, 255);
cv::Scalar bgr_green(0, 255, 0);
cv::Scalar bgr_yellow(0, 255, 255);

	int redLowH = hsv_red_min_upper.val[0];
	int redHighH = hsv_red_max_upper.val[0];
	int redLowS = hsv_red_min_upper.val[1];
	int redHighS = hsv_red_max_upper.val[1];
	int redLowV = hsv_red_min_upper.val[2];
	int redHighV = hsv_red_max_upper.val[2];

	int yellowLowH = hsv_yellow_min.val[0];
	int yellowHighH = hsv_yellow_max.val[0];
	int yellowLowS = hsv_yellow_min.val[1];
	int yellowHighS = hsv_yellow_max.val[1];
	int yellowLowV = hsv_yellow_min.val[2];
	int yellowHighV = hsv_yellow_max.val[2];

	int greenLowH = hsv_green_min.val[0];
	int greenHighH = hsv_green_max.val[0];
	int greenLowS = hsv_green_min.val[1];
	int greenHighS = hsv_green_max.val[1];
	int greenLowV = hsv_green_min.val[2];
	int greenHighV = hsv_green_max.val[2];

	int purpleLowH = hsv_purple_min.val[0];
	int purpleHighH = hsv_purple_max.val[0];
	int purpleLowS = hsv_purple_min.val[1];
	int purpleHighS = hsv_purple_max.val[1];
	int purpleLowV = hsv_purple_min.val[2];
	int purpleHighV = hsv_purple_max.val[2];

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

void trackObjects(cv::Mat &filteredImg, cv::Mat &drawing, cv::Scalar colour, cv::Point &position){
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(filteredImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	std::vector<cv::RotatedRect> boundingBox;
	std::vector<cv::Point> centrePoints;
	double maxArea = 0;
	for (int i = 0; i < contours.size(); i++){
		cv::RotatedRect rect = cv::minAreaRect(contours[i]);

		if (cv::contourArea(contours[i]) >= 300){
			boundingBox.push_back(rect);
			centrePoints.push_back(rect.center);
			if (cv::contourArea(contours[i]) > maxArea){
				maxArea = cv::contourArea(contours[i]);
				position = rect.center;
			}
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

void getChairPosition(cv::Mat origImg, cv::Point &fl, cv::Point &fr, cv::Point &bl, cv::Point &br){
	// convert input image to HSV
	cv::Mat hsvImg;
	cv::medianBlur(origImg, origImg, 25);
	cv::cvtColor(origImg, hsvImg, cv::COLOR_BGR2HSV);

	// threshold the HSV image, keep only the red pixels
	cv::Mat threshold;
	cv::Mat points = cv::Mat::zeros(hsvImg.size(), CV_8UC3);

	// find red
	//cv::Mat lower_red_hsv_range;
	//cv::Mat upper_red_hsv_range;
	//cv::inRange(hsvImg, hsv_red_min_lower, hsv_red_max_lower, lower_red_hsv_range);
	cv::inRange(hsvImg, cv::Scalar(redLowH, redLowS, redLowV), cv::Scalar(redHighH, redHighS, redHighV), threshold);
	//cv::addWeighted(lower_red_hsv_range, 1.0, upper_red_hsv_range, 1.0, 0.0, threshold);
	morphImg(threshold);
	cv::Point redPos = cv::Point(-1, -1);
	trackObjects(threshold, points, bgr_red, redPos);

	// find purple
	cv::cvtColor(origImg, hsvImg, cv::COLOR_BGR2HSV);
	cv::inRange(hsvImg, cv::Scalar(purpleLowH, purpleLowS, purpleLowV), cv::Scalar(purpleHighH, purpleHighS, purpleHighV), threshold);
	morphImg(threshold);
	cv::Point purplePos = cv::Point(-1, -1);
	trackObjects(threshold, points, bgr_purple, purplePos);

	// find green
	cv::cvtColor(origImg, hsvImg, cv::COLOR_BGR2HSV);
	cv::inRange(hsvImg, cv::Scalar(greenLowH, greenLowS, greenLowV), cv::Scalar(greenHighH, greenHighS, greenHighV), threshold);
	morphImg(threshold);
	cv::Point greenPos = cv::Point(-1, -1);
	trackObjects(threshold, points, bgr_green, greenPos);

	// find yellow
	cv::cvtColor(origImg, hsvImg, cv::COLOR_BGR2HSV);
	cv::inRange(hsvImg, cv::Scalar(yellowLowH, yellowLowS, yellowLowV), cv::Scalar(yellowHighH, yellowHighS, yellowHighV), threshold);
	cv::Point yellowPos = cv::Point(-1, -1);
	trackObjects(threshold, points, bgr_yellow, yellowPos);

	// display images
	cv::imshow("original", origImg);
	//cv::imshow("points", points);

	fl = greenPos;
	fr = purplePos;
	bl = redPos;
	br = yellowPos;
}

bool readCapture(cv::VideoCapture cap, cv::Mat img){
	bool success = cap.read(img);
	if (!success){
		std::cout << "read unsuccessful" << std::endl;
	}
	return success;
}

ChairFrame getChairFrame(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point p4){
	ChairCoord fl = ChairCoord(p1.x, p1.y);
	ChairCoord fr = ChairCoord(p2.x, p2.y);
	ChairCoord bl = ChairCoord(p3.x, p3.y);
	ChairCoord br = ChairCoord(p4.x, p4.y);
	return ChairFrame(fl, fr, bl, br);
}

int getNumPointsFound(cv::Point &p1, cv::Point &p2, cv::Point &p3, cv::Point &p4){
	int numPointsFound = 0;
	if (p1.x != -1 && p1.y != -1){
		numPointsFound++;
	}
	else{
		p1.x = 0;
		p1.y = 0;
	}
	if (p2.x != -1 && p2.y != -1){
		numPointsFound++;
	}
	else{
		p2.x = 0;
		p2.y = 0;
	}
	if (p3.x != -1 && p3.y != -1){
		numPointsFound++;
	}
	else{
		p3.x = 0;
		p3.y = 0;
	}
	if (p4.x != -1 && p4.y != -1){
		numPointsFound++;
	}
	else{
		p4.x = 0;
		p4.y = 0;
	}
	return numPointsFound;
}

void drawMappedPoints(ChairFrame cf, cv::Mat &map){
	ChairCoord fl = cf.getFl();
	ChairCoord fr = cf.getFr();
	ChairCoord bl = cf.getBl();
	ChairCoord br = cf.getBr();
	std::cout << "(" << fl.x << "," << fl.y << ") " << "(" << fr.x << "," << fr.y << ") " << "(" << bl.x << "," << bl.y << ") " << "(" << br.x << "," << br.y << ")" << std::endl;
	cv::Point p1 = cv::Point(map.rows / 2 + fl.x * 3, map.cols / 2 - fl.y * 3);
	cv::Point p2 = cv::Point(map.rows / 2 + fr.x * 3, map.cols / 2 - fr.y * 3);
	cv::Point p3 = cv::Point(map.rows / 2 + bl.x * 3, map.cols / 2 - bl.y * 3);
	cv::Point p4 = cv::Point(map.rows / 2 + br.x * 3, map.cols / 2 - br.y * 3);
	cv::circle(map, p1, 3, cv::Scalar(255, 255, 255), -1);
	cv::circle(map, p2, 3, cv::Scalar(255, 255, 255), -1);
	cv::circle(map, p3, 3, cv::Scalar(255, 255, 255), -1);
	cv::circle(map, p4, 3, cv::Scalar(255, 255, 255), -1);
	cv::putText(map, "fl", cv::Point(p1.x, p1.y - 40), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar::all(255), 2, 8);
	cv::putText(map, "fr", cv::Point(p2.x, p2.y - 40), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar::all(255), 3, 8);
	cv::putText(map, "bl", cv::Point(p3.x, p3.y - 40), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar::all(255), 3, 8);
	cv::putText(map, "br", cv::Point(p4.x, p4.y - 40), CV_FONT_HERSHEY_SIMPLEX, 2, cv::Scalar::all(255), 3, 8);
}

void createControls(){
	cv::namedWindow("red", CV_WINDOW_AUTOSIZE);
	cv::createTrackbar("LowH", "red", &redLowH, 179);
	cv::createTrackbar("HighH", "red", &redHighH, 179);
	cv::createTrackbar("LowS", "red", &redLowS, 255);
	cv::createTrackbar("HighS", "red", &redHighS, 255);
	cv::createTrackbar("LowV", "red", &redLowV, 255);
	cv::createTrackbar("HighV", "red", &redHighV, 255);

	cv::namedWindow("yellow", CV_WINDOW_AUTOSIZE);
	cv::createTrackbar("LowH", "yellow", &yellowLowH, 179);
	cv::createTrackbar("HighH", "yellow", &yellowHighH, 179);
	cv::createTrackbar("LowS", "yellow", &yellowLowS, 255);
	cv::createTrackbar("HighS", "yellow", &yellowHighS, 255);
	cv::createTrackbar("LowV", "yellow", &yellowLowV, 255);
	cv::createTrackbar("HighV", "yellow", &yellowHighV, 255);

	cv::namedWindow("green", CV_WINDOW_AUTOSIZE);
	cv::createTrackbar("LowH", "green", &greenLowH, 179);
	cv::createTrackbar("HighH", "green", &greenHighH, 179);
	cv::createTrackbar("LowS", "green", &greenLowS, 255);
	cv::createTrackbar("HighS", "green", &greenHighS, 255);
	cv::createTrackbar("LowV", "green", &greenLowV, 255);
	cv::createTrackbar("HighV", "green", &greenHighV, 255);

	cv::namedWindow("purple", CV_WINDOW_AUTOSIZE);
	cv::createTrackbar("LowH", "purple", &purpleLowH, 179);
	cv::createTrackbar("HighH", "purple", &purpleHighH, 179);
	cv::createTrackbar("LowS", "purple", &purpleLowS, 255);
	cv::createTrackbar("HighS", "purple", &purpleHighS, 255);
	cv::createTrackbar("LowV", "purple", &purpleLowV, 255);
	cv::createTrackbar("HighV", "purple", &purpleHighV, 255);
}

int main(int argc, char **argv) {

	// exit if capture wasn't successful
	// try capturing image from the webcam
	cv::VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 2000);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 2000);

	if (!cap.isOpened()){
		std::cout << "webcam not on" << std::endl;
		return -1;
	}

	std::string word;
	std::cout << "Type \"start\" to capture the base frame" << std::endl;
	while (word.compare("start") != 0){
		std::getline(std::cin, word);
	}
	std::cout << "Capturing base frame" << std::endl;

	// create control windows
	//createControls();

	// find the base frame with the initial image processing
	cv::Mat origImg;
	cap.read(origImg);

	cv::Point greenPos, purplePos, redPos, yellowPos;
	getChairPosition(origImg, greenPos, purplePos, redPos, yellowPos);
	ChairFrame baseFrame = getChairFrame(greenPos, purplePos, redPos, yellowPos);
	ChairProcessor cp = ChairProcessor(baseFrame);

	// capture loop
	while (true){

		//break the loop if capture isn't successful
		if (!readCapture(cap, origImg)){
			break;
		}

		greenPos = cv::Point(-1, -1);
		purplePos = cv::Point(-1, -1);
		redPos = cv::Point(-1, -1);
		yellowPos = cv::Point(-1, -1);

		getChairPosition(origImg, greenPos, purplePos, redPos, yellowPos);
		std::cout << greenPos.x << "," << greenPos.y << " " << purplePos.x << "," << purplePos.y << " " << redPos.x << "," << redPos.y << " " << yellowPos.x << "," << yellowPos.y << std::endl;

		int numPointsFound = getNumPointsFound(greenPos, purplePos, redPos, yellowPos);

		std::cout << "Found " << numPointsFound << " points" << std::endl;

		// do path finding if 2 or more points are found
		if (numPointsFound >= 2){
			ChairFrame cf = getChairFrame(greenPos, purplePos, redPos, yellowPos);
			cp.processCurrentFrame(cf);
			ChairFrame mappedFrame = cp.getMappedCurrentFrame();
			cv::Mat mappedPoints(600, 600, CV_8UC3, cv::Scalar(0,0,0));
			drawMappedPoints(mappedFrame, mappedPoints);
			//cv::imshow("mappedPoints", mappedPoints);
		}

		// exit with esc key
		int key = cv::waitKey(30);
		std::cout << "key entered is " << key << std::endl;
		if (key == 27){
			break;
		}
	}
	return 0;
}