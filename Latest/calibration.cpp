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
#include <cv.h>
using namespace cv;
using namespace std;

Scalar hsv_min_red(130, 100, 100);
Scalar hsv_max_red(179, 140, 140);
Scalar hsv_min_blue(140, 0, 40);
Scalar hsv_max_blue(179, 100, 100);
Scalar hsv_min_green(50, 60, 40);
Scalar hsv_max_green(80, 190, 90);
Scalar hsv_min_yellow(20, 60, 160);
Scalar hsv_max_yellow(40, 100, 200);
Scalar bgr_red(0, 0, 255);
Scalar bgr_blue(255, 0, 0);
Scalar bgr_green(0, 255, 0);
Scalar bgr_yellow(0, 255, 255);

void drawObjects(vector<RotatedRect> boundingBox, vector<Point> centrePoints, Mat &drawing, Scalar colour){
	// draw the rotated rect and centre point
	for (int i = 0; i < boundingBox.size(); i++){
		Point2f corners[4];
		boundingBox[i].points(corners);
		line(drawing, corners[0], corners[1], colour);
		line(drawing, corners[1], corners[2], colour);
		line(drawing, corners[2], corners[3], colour);
		line(drawing, corners[3], corners[0], colour);
		circle(drawing, centrePoints[i], 3, colour, -1);
	}
}

bool isRectangle(vector<Point> contour){
    double perim = arcLength(contour, true);
    vector<Point> approx;
    approxPolyDP(contour, approx, 0.06 * perim, true);
	cout << "contour points found: " << approx.size() << endl;
    if(approx.size() == 4){
        Rect rect = boundingRect(approx);
        if(rect.height > rect.width)
            return true;
    }
    
    return false;
}

void trackObjects(Mat &img_filtered, Mat &drawing, Scalar colour){

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(img_filtered, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//zero matrix to draw on
	drawing = Mat::zeros(img_filtered.size(), CV_8UC3);

	vector<RotatedRect> boundingBox;
	vector<Point> centrePoints;
	    vector<RotatedRect> maxBox;
    vector<Point> maxPoint;
    double maxArea = 0;
	for (int i = 0; i < contours.size(); i++){
		RotatedRect rect = minAreaRect(contours[i]);

		if (contourArea(contours[i]) >= 300 && contourArea(contours[i]) > maxArea){
			boundingBox.push_back(rect);
			centrePoints.push_back(rect.center);
			if (isRectangle(contours[i])){
                maxBox.clear();
                maxBox.push_back(rect);
                maxPoint.clear();
                maxPoint.push_back(rect.center);
                maxArea = contourArea(contours[i]);
			}
		}
	}
	drawObjects(maxBox, maxPoint, drawing, colour);
	//drawObjects(boundingBox, centrePoints, drawing, colour);
}

void morphImg(Mat &img){
	// use erosion and dilation to remove noise
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(10, 10));

	erode(img, img, erodeElement);
	erode(img, img, erodeElement);

	dilate(img, img, dilateElement);
	dilate(img, img, dilateElement);
}

int main(int argc, char **argv) {

	// try capturing image from the webcam
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 2000);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 2000);

	// exit if capture wasn't successful
	if (!cap.isOpened()){
		cout << "webcam not on" << endl;
		return -1;
	}

	Mat origImg;

	namedWindow("Control", CV_WINDOW_AUTOSIZE);
	int iLowH = 50;
	int iHighH = 70;
	int iLowS = 60;
	int iHighS = 150;
	int iLowV = 50;
	int iHighV = 170;

	createTrackbar("LowH", "Control", &iLowH, 179);
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255);
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);
	createTrackbar("HighV", "Control", &iHighV, 255);

	int iLowHUpper = 0;
	int iHighHUpper = 0;
	int iLowSUpper = 0;
	int iHighSUpper = 0;
	int iLowVUpper = 0;
	int iHighVUpper = 0;
	
	namedWindow("ControlUpper", CV_WINDOW_AUTOSIZE);
	createTrackbar("LowH", "ControlUpper", &iLowHUpper, 179);
	createTrackbar("HighH", "ControlUpper", &iHighHUpper, 179);

	createTrackbar("LowS", "ControlUpper", &iLowSUpper, 255);
	createTrackbar("HighS", "ControlUpper", &iHighSUpper, 255);

	createTrackbar("LowV", "ControlUpper", &iLowVUpper, 255);
	createTrackbar("HighV", "ControlUpper", &iHighVUpper, 255);

	// capture loop
	while (true){

		//break the loop if capture isn't successful
		bool success = cap.read(origImg);
		if (!success){
			cout << "read unccessful" << endl;
			break;
		}
		imshow("original", origImg);

		// convert input image to HSV
		medianBlur(origImg, origImg, 19);
		imshow("blurred", origImg);
		Mat hsvImg;
		cvtColor(origImg, hsvImg, COLOR_BGR2HSV);
		
		// threshold the HSV image, keep only the red pixels
		// find red first
		Mat threshold;
		Mat threshold2;
		inRange(hsvImg, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshold);
		inRange(hsvImg, Scalar(iLowHUpper, iLowSUpper, iLowVUpper), Scalar(iHighHUpper, iHighSUpper, iHighVUpper), threshold2);
		addWeighted(threshold, 1, threshold2, 1, 0, threshold);
		imshow("unmorphed", threshold);
		morphImg(threshold);
		imshow("morphed", threshold);
		Mat test;
		trackObjects(threshold, test, bgr_red);

		// display images
		imshow("test", test);

		if (waitKey(30) == 27){
			break;
		}
	}
	return 0;
}