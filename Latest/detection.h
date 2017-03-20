#ifndef DETECTION
#define DETECTION

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
#include <sqlite3.h>

class Detection {
	public:
		Detection::Detection();
		int Detection::detectionFunc();
	private:
		int redLowH;
		int redHighH;
		int redLowS;
		int redHighS;
		int redLowV;
		int redHighV;

		int redLowHUpper;
		int redHighHUpper;
		int redLowSUpper;
		int redHighSUpper;
		int redLowVUpper;
		int redHighVUpper;

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
		void Detection::drawObjects(vector<RotatedRect> boundingBox, vector<Point> centrePoints, Mat &drawMat, Scalar colour);
		bool Detection::isRectangle(vector<Point> contour);
		void Detection::showFilteredImg(Mat img, Object obj);
		void Detection::trackObjects(Mat &filteredImg, Mat &drawMat, Object &obj);
		void Detection::setHsvRange(Object &obj);
		void Detection::morphImg(Mat &img);
		void Detection::getObjects(Mat hsvImg, Object &obj, Mat &points);
		bool Detection::readCapture(VideoCapture cap, Mat img);
		ChairFrame Detection::getChairFrame(Point p1, Point p2, Point p3, Point p4);
		int Detection::getNumPointsFound(Object &blue, Object &green, Object &yellow, Object &red);
		void Detection::drawMappedPoints(Mat &map, ChairProcessor cp);
		void Detection::sqlStatement(time_t timeIn, int sessionID, string &query);
		void Detection::createControls(Object blue, Object green, Object yellow, Object red);



};
#endif