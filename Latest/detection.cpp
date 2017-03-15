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
#include "Object.h"
#include <thread>
#include <chrono>
using namespace cv;
using namespace std;

int redLowH;
int redHighH;
int redLowS;
int redHighS;
int redLowV;
int redHighV;

int yellowLowH;
int yellowHighH;
int yellowLowS;
int yellowHighS;
int yellowLowV;
int yellowHighV;

int greenLowH;
int greenHighH;
int greenLowS;
int greenHighS;
int greenLowV;
int greenHighV;

int blueLowH;
int blueHighH;
int blueLowS;
int blueHighS;
int blueLowV;
int blueHighV;

void drawObjects(vector<RotatedRect> boundingBox, vector<Point> centrePoints, Mat &drawMat, Scalar colour){
	// draw the rotated rect and centre point
	for (int i = 0; i < boundingBox.size(); i++){
		Point2f corners[4];
		boundingBox[i].points(corners);
		line(drawMat, corners[0], corners[1], colour);
		line(drawMat, corners[1], corners[2], colour);
		line(drawMat, corners[2], corners[3], colour);
		line(drawMat, corners[3], corners[0], colour);
		circle(drawMat, centrePoints[i], 3, colour, -1);
	}
}

bool isRectangle(vector<Point> contour){
    double perim = arcLength(contour, true);
    vector<Point> approx;
    approxPolyDp(contour, approx, 0.04 * perim. true);
    if(approx.size == 4){
        Rect rect = boudningRect(approx);
        if(rect.height > rect.width)
            return true;
    }
    
    return false;
}

void trackObjects(Mat &filteredImg, Mat &drawMat, Object &obj){
	vector<vector<Point> > contours;
	findContours(filteredImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
	vector<RotatedRect> boundingBox;
	vector<Point> centrePoints;
    vector<RotatedRect> maxBox;
    vector<Point> maxPoint;
	double maxArea = 0;
	for (int i = 0; i < contours.size(); i++){
		RotatedRect rect = minAreaRect(contours[i]);

		if (contourArea(contours[i]) >= 300 && isRectangle(contours[i])){
			boundingBox.push_back(rect);
			centrePoints.push_back(rect.center);
			if (contourArea(contours[i]) > maxArea){
                maxBox.clear;
                maxBox.push_back(rect);
                maxPoint.clear;
                maxPoint.push_back(rect);
				maxArea = contourArea(contours[i]);
				obj.setPoint(rect.center);
			}
		}
	}
    drawObjects(maxBox, maxPoint, drawMat, obj.getColourBgr());
	//drawObjects(boundingBox, centrePoints, drawMat, obj.getColourBgr());
}

void setHsvRange(Object &obj){
    if(obj.getColour() == "green"){
        obj.setHsvMin(Scalar(greenLowH, greenLowS, greenLowV));
        obj.setHsvMax(Scalar(greenHighH, greenHighS, greenHighV));
    }
    else if(obj.getColour() == "blue"){
        obj.setHsvMin(Scalar(blueLowH, blueLowS, blueLowV));
        obj.setHsvMax(Scalar(blueHighH, blueHighS, blueHighV));
    }
    else if(obj.getColour() == "red"){
        obj.setHsvMin(Scalar(redLowH, redLowS, redLowV));
        obj.setHsvMax(Scalar(redHighH, redHighS, redHighV));
    } 
    else{
        obj.setHsvMin(Scalar(yellowLowH, yellowLowS, yellowLowV));
        obj.setHsvMax(Scalar(yellowHighH, yellowHighS, yellowHighV));
    }
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

void getObjects(Mat hsvImg, Object &obj, Mat &points){
	// threshold matrix
	Mat threshold;

    // refresh the threshold range
    setHsvRange(obj);
    
	// threshold the image with hsv range
	inRange(hsvImg, obj.getHsvMin(), obj.getHsvMax(), threshold);
    
    // use morphological transformations to further remove noise
	morphImg(threshold);
    
    // initialize the point to (-1, -1)
	obj.setPoint(Point(-1, -1));
    
    // finally track the objects
	trackObjects(threshold, points, obj);
}

bool readCapture(VideoCapture cap, Mat img){
	bool success = cap.read(img);
	if (!success){
		cout << "read unsuccessful" << endl;
	}
	return success;
}

ChairFrame getChairFrame(Point p1, Point p2, Point p3, Point p4){
	ChairCoord fl = ChairCoord(p1.x, p1.y);
	ChairCoord fr = ChairCoord(p2.x, p2.y);
	ChairCoord bl = ChairCoord(p3.x, p3.y);
	ChairCoord br = ChairCoord(p4.x, p4.y);
	return ChairFrame(fl, fr, bl, br);
}

int getNumPointsFound(Object &blue, Object &green, Object &yellow, Object &red){
	int numPointsFound = 0;
	Point p1 = blue.getPoint();	
	Point p2 = green.getPoint();
	Point p3 = yellow.getPoint();
	Point p4 = red.getPoint();
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
	blue.setPoint(p1);
	green.setPoint(p2);
	yellow.setPoint(p3);
	red.setPoint(p4);
	return numPointsFound;
}

void drawMappedPoints(ChairFrame cf, Mat &map){
	ChairCoord fl = cf.getFl();
	ChairCoord fr = cf.getFr();
	ChairCoord bl = cf.getBl();
	ChairCoord br = cf.getBr();
    cout << "mapped points:" << endl;
	cout << "(" << fl.x << "," << fl.y << ") " << "(" << fr.x << "," << fr.y << ") " << "(" << bl.x << "," << bl.y << ") " << "(" << br.x << "," << br.y << ")" << endl;
	Point p1 = Point(map.rows / 2 + fl.x * 3, map.cols / 2 - fl.y * 3);
	Point p2 = Point(map.rows / 2 + fr.x * 3, map.cols / 2 - fr.y * 3);
	Point p3 = Point(map.rows / 2 + bl.x * 3, map.cols / 2 - bl.y * 3);
	Point p4 = Point(map.rows / 2 + br.x * 3, map.cols / 2 - br.y * 3);
	circle(map, p1, 3, Scalar(255, 255, 255), -1);
	circle(map, p2, 3, Scalar(255, 255, 255), -1);
	circle(map, p3, 3, Scalar(255, 255, 255), -1);
	circle(map, p4, 3, Scalar(255, 255, 255), -1);
	putText(map, "fl", Point(p1.x, p1.y - 40), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar::all(255), 2, 8);
	putText(map, "fr", Point(p2.x, p2.y - 40), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar::all(255), 3, 8);
	putText(map, "bl", Point(p3.x, p3.y - 40), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar::all(255), 3, 8);
	putText(map, "br", Point(p4.x, p4.y - 40), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar::all(255), 3, 8);
}

void createControls(Object blue, Object green, Object yellow, Object red){

    //initialize control variables
    Scalar redHsvMin = red.getHsvMin();
    Scalar redHsvMax = red.getHsvMax();
    redLowH = redHsvMin[0];
    redHighH = redHsvMax[0];
    redLowS = redHsvMin[1];
    redHighS = redHsvMax[1];
    redLowV = redHsvMin[2];
    redHighV = redHsvMax[2];

    Scalar blueHsvMin = blue.getHsvMin();
    Scalar blueHsvMax = blue.getHsvMax();
    blueLowH = blueHsvMin[0];
    blueHighH = blueHsvMax[0];
    blueLowS = blueHsvMin[1];
    blueHighS = blueHsvMax[1];
    blueLowV = blueHsvMin[2];
    blueHighV = blueHsvMax[2];
    
    Scalar greenHsvMin = green.getHsvMin();
    Scalar greenHsvMax = green.getHsvMax();
    greenLowH = greenHsvMin[0];
    greenHighH = greenHsvMax[0];
    greenLowS = greenHsvMin[1];
    greenHighS = greenHsvMax[1];
    greenLowV = greenHsvMin[2];
    greenHighV = greenHsvMax[2];
    
    Scalar yellowHsvMin = yellow.getHsvMin();
    Scalar yellowHsvMax = yellow.getHsvMax();
    yellowLowH = yellowHsvMin[0];
    yellowHighH = yellowHsvMax[0];
    yellowLowS = yellowHsvMin[1];
    yellowHighS = yellowHsvMax[1];
    yellowLowV = yellowHsvMin[2];
    yellowHighV = yellowHsvMax[2];
    
    // create control windows
	namedWindow("red", CV_WINDOW_AUTOSIZE);
	createTrackbar("LowH", "red", &redLowH, 179);
	createTrackbar("HighH", "red", &redHighH, 179);
	createTrackbar("LowS", "red", &redLowS, 255);
	createTrackbar("HighS", "red", &redHighS, 255);
	createTrackbar("LowV", "red", &redLowV, 255);
	createTrackbar("HighV", "red", &redHighV, 255);

	namedWindow("yellow", CV_WINDOW_AUTOSIZE);
	createTrackbar("LowH", "yellow", &yellowLowH, 179);
	createTrackbar("HighH", "yellow", &yellowHighH, 179);
	createTrackbar("LowS", "yellow", &yellowLowS, 255);
	createTrackbar("HighS", "yellow", &yellowHighS, 255);
	createTrackbar("LowV", "yellow", &yellowLowV, 255);
	createTrackbar("HighV", "yellow", &yellowHighV, 255);

	namedWindow("green", CV_WINDOW_AUTOSIZE);
	createTrackbar("LowH", "green", &greenLowH, 179);
	createTrackbar("HighH", "green", &greenHighH, 179);
	createTrackbar("LowS", "green", &greenLowS, 255);
	createTrackbar("HighS", "green", &greenHighS, 255);
	createTrackbar("LowV", "green", &greenLowV, 255);
	createTrackbar("HighV", "green", &greenHighV, 255);

	namedWindow("blue", CV_WINDOW_AUTOSIZE);
	createTrackbar("LowH", "blue", &blueLowH, 179);
	createTrackbar("HighH", "blue", &blueHighH, 179);
	createTrackbar("LowS", "blue", &blueLowS, 255);
	createTrackbar("HighS", "blue", &blueHighS, 255);
	createTrackbar("LowV", "blue", &blueLowV, 255);
	createTrackbar("HighV", "blue", &blueHighV, 255);
}

int main(int argc, char **argv) {

	// exit if capture wasn't successful
	// try capturing image from the webcam
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 2000);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 2000);

	if (!cap.isOpened()){
		cout << "webcam not on" << endl;
		return -1;
	}

	string word;
	cout << "Type \"start\" to capture the base frame" << endl;
	while (word.compare("start") != 0){
		getline(cin, word);
	}
	cout << "Capturing base frame" << endl;

	// find the base frame with the initial image processing
	Mat origImg;
	cap.read(origImg);
    
    // blur the image to get rid of noise
	medianBlur(origImg, origImg, 25);
    
    // convert input image to HSV
	Mat hsvImg;
	cvtColor(origImg, hsvImg, COLOR_BGR2HSV);
    
	// get the base frame
	Object blue = Object("blue");
	Object green = Object("green");
	Object yellow = Object("yellow");
	Object red = Object("red");
    
    // create control windows
	createControls(blue, green, yellow, red);
    
    // matrix to draw points on
    Mat points = Mat::zeros(hsvImg.size(), CV_8UC3);
    
    // get objects with the corresponding colour
	thread t1(getObjects, hsvImg, ref(blue), ref(points));
    thread t2(getObjects, hsvImg, ref(green), ref(points));
    thread t3(getObjects, hsvImg, ref(yellow), ref(points));    
    thread t4(getObjects, hsvImg, ref(red), ref(points));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
	ChairFrame baseFrame = getChairFrame(blue.getPoint(), green.getPoint(), yellow.getPoint(), red.getPoint());
	ChairProcessor cp = ChairProcessor(baseFrame);

	// capture loop
	while (true){

		//break the loop if capture isn't successful
		if (!readCapture(cap, origImg)){
			break;
		}
	imshow("original", origImg);
        
        medianBlur(origImg, origImg, 25);
	imshow("blurred", origImg);
        cvtColor(origImg, hsvImg, COLOR_BGR2HSV);
        
        // reset points
		green.setPoint(Point(-1, -1));
		blue.setPoint(Point(-1, -1));
		red.setPoint(Point(-1, -1));
		yellow.setPoint(Point(-1, -1));
        
        // matrix to draw points on
        points = Mat::zeros(hsvImg.size(), CV_8UC3);
    	thread t1(getObjects, hsvImg, ref(blue), ref(points));
    	thread t2(getObjects, hsvImg, ref(green), ref(points));
    	thread t3(getObjects, hsvImg, ref(yellow), ref(points));
    	thread t4(getObjects, hsvImg, ref(red), ref(points));

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        
	imshow("points", points);

        cout << "unmapped points:" << endl;
		cout << blue.getPoint().x << "," << blue.getPoint().y << " " << green.getPoint().x << "," << green.getPoint().y << " " << yellow.getPoint().x << "," << yellow.getPoint().y << " " << red.getPoint().x << "," << red.getPoint().y << endl;

		int numPointsFound = getNumPointsFound(blue, green, yellow, red);

		cout << "Found " << numPointsFound << " points" << endl;

		// do path finding if 2 or more points are found
		if (numPointsFound >= 2){
			ChairFrame cf = getChairFrame(blue.getPoint(), green.getPoint(), yellow.getPoint(), red.getPoint());
			cp.processCurrentFrame(cf);
			ChairFrame mappedFrame = cp.getMappedCurrentFrame();
			Mat mappedPoints(600, 600, CV_8UC3, Scalar(0,0,0));
			drawMappedPoints(mappedFrame, mappedPoints);
			imshow("mappedPoints", mappedPoints);
		}

		// exit with esc key
		int key = waitKey(30);
		cout << "key entered is " << key << endl;
		if (key == 27){
			break;
		}
	}
	return 0;
}