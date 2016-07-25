#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iomanip>
#include <sqlite3.h>
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include <unistd.h>0

using namespace std;
using namespace cv;

enum Direction { left, right, straight, still };
Direction d = left

vector<Point> processImage(Mat bgr_image, int low1, int low2, int low3, int high1, int high2, int high3){
	// Show the original images
	namedWindow("Original Image", WINDOW_NORMAL);
	imshow("Original Image", bgr_image);

	Mat orig_image = bgr_image.clone();

	// Convert input image to HSV
	Mat hsv_image;
	cvtColor(bgr_image, hsv_image, COLOR_BGR2HSV);

	// Threshold the HSV image
	Mat hue_range;
	inRange(hsv_image, Scalar(low1, low2, low3), Scalar(high1, high2, high3), hue_range);
	
	// Combine the above two images
	Mat hue_image = hue_range.clone();

	GaussianBlur(hue_image, hue_image, Size(5, 5), 2, 2);

	// Show the filtered images
	namedWindow("Threshold Image", WINDOW_NORMAL);
	imshow("Threshold Image", hue_image);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(hue_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//draw contours
	Mat drawing = Mat::zeros(hue_image.size(), CV_8UC3);
	Mat contourDrawing = Mat::zeros(hue_image.size(), CV_8UC3);;
	drawContours(contourDrawing, contours, -1, Scalar(255, 255, 255));

	//detect centre points
	vector<RotatedRect> boundingBox;
	vector<Point> centrePoints;
	for (int i = 0; i < contours.size(); i++){
		RotatedRect rect = minAreaRect(contours[i]);
		Point2f corners[4];
		rect.points(corners);
		bool isDetected = true;
		Point2f prevCorner;
		prevCorner = corners[0];

		for (int j = 1; j < 4; j++){
			if (corners[j] == prevCorner)
				isDetected = false;
		}
		if (isDetected && (contourArea(contours[i]) >= 1500)){
			boundingBox.push_back(rect);
			centrePoints.push_back(rect.center);
		}
	}
	// draw the rotated rect and centre point
	for (int i = 0; i < boundingBox.size(); i++){
		Point2f corners[4];
		boundingBox[i].points(corners);
		line(drawing, corners[0], corners[1], Scalar(255, 255, 255), 2);
		line(drawing, corners[1], corners[2], Scalar(255, 255, 255), 2);
		line(drawing, corners[2], corners[3], Scalar(255, 255, 255), 2);
		line(drawing, corners[3], corners[0], Scalar(255, 255, 255), 2);
		circle(drawing, centrePoints[i], 3, Scalar(255, 255, 255), -1);
	}

	// display on named windows
	namedWindow("origFinal", WINDOW_NORMAL);
	imshow("origFinal", drawing);
	waitKey(1);
	return centrePoints;
}

Direction pathToOrigin(vector<Point> originRed, vector<Point> originGreen, vector<Point> currentRed, vector<Point> currentGreen) {
	Direction d = still;
	int numPoints, avgX, chairSize, originAvgX, relativeX;
	numpoints = currentRed.size() + currentGreen.size();
	charsize = 0;

	originAvgX = (originRed[0].x + originRed[1].x) / 2;
	
	//2 points are detected
	if (numPoints == 2) {
		//both 2 red points are detected, facing towards the camera
		if (currentRed.size() == 2) {
			//find the relative x position and chair size indicator
			avgX = (currentRed[0].x + currentRed[1].x) / 2;
			chairSize = abs(currentRed[0].x - currentRed[1].x);
			relativeX = avgX - originAvgX;

			//rotate the chair to the left if the chair is to the left of the camera
			if (relativeX < - (chairSize / 10)) {
				d = left;
				return d;
			}
			//rotate the chair to the right if the chair is to the right of the camera
			else if (relativeX > (chairSize / 10)) {
				d = right;
				return d;
			}
			//move straight if the chair is already at the centre
			else
			{	
				//move only until the chair is close to the origin
				if(currentRed[0].y - originRed[0].y < chairSize/5)
				d = straight;
				return d;
			}
		}
		//1 red, 1 green points are detected, facing left or right
		else {
			avgX = (currentRed[0].x + currentGreen[0].x) / 2;
			chairSize = abs(currentRed[0].x - currentGreen[0].x);
			relativeX = avgX - originAvgX;
			
			//chair is already at the centre, facing left or right
			if (abs(relativeX) < chairSize / 10) {
				//rotate the chair to the right if it's facing left
				if (currentRed[0].x - currentGreen[0].x > 0) {
					d = right;
					return d;
				}
				//rotate the chair to the left if it's facing right
				else {
					d = left;
					return d;
				}
			}
			//chair is to the left of the centre, facing left or right 
			else if (relativeX < 0) {
				//move the chair straight to the centre if it's facing right
				if (currentRed[0].x - currentGreen[0].x > 0) {
					d = straight;
					return d;
				}
				//rotate the chair to the right towards the centre if it's facing left
				else {
					d = right;
					return d;
				}
			}
			//chair is to the right of the centre, facing left or right
			else{
				//rotate the chair to the left towards the centre if it's facing right 
				if (currentRed[0].x - currentGreen[0].x > 0) {
					d = left;
					return d;
				}
				//move the to the centre if it's facing left
				else {
					d = straight;
					return d;
				}
			}
		}
	}
	//3 points are found, 2 reds and 1 green. Chair is facing southwest or southeast direction.
	else if (numPoints == 3) {
		avgX = (currentRed[0].x + currentRed[1].x + currentGreen[0].x) / 3;
		chairSize = max(max(abs(currentRed[0].x - currentGreen[0].x), abs(currentred[1].x - currentGreen[0].x)), abs(currentRed[0].x - currentred[1].x));
		relativeX = avgX - originAvgX;
		//find the lower red point that lines up with the green point detected
		point lowerRed;
		if(currentRed[0].y < currentRed[1].y)
			lowerRed = currentRed[0];
		else
			lowerRed = currentRed[1];

		//Chair is to the right of the centre
		if (relativeX > (chairSize / 10)) {
			//move the chair towards the centre if it's facing the centre
			if(lowerRed.x < currentGreen[0].x && abs(lowerRed.y - currentGreen[0].y) < (chairSize / 20)){
				d = straight;
				return d;
			}
				
			//rotate the chair to the right towards the centre
			d = right;
			return d;
		}
		//Chair is to the left of the centre
		else if(relativeX < -(chairSize / 10)){
			//move the chair towards the centre if it's facing the centre
			if(lowerRed.x > currentGreen[0].x && abs(lowerRed.y - currentGreen[0].y) < (chairSize / 20)){
				d = straight;
				return d;
			}
			
			//rotate the chair to the left towards the centre
			d = left;
			return d;
		}
		//Chair is at the centre
		else{
			//rotate to the right if the chair is facing southwest
			if(currentRed[0].x - currentGreen[0].x < -(chairSize / 10)){
				d = left;
				return d;
			}
			//rotate to the left if the chair is facing southeast
			if(currentRed[0].x - currentGreen[0].x > (chairSize / 10)){
				d = right;
				return d;
			}
		}
	}
	//all 4 points are found
	else if (numPoints == 4) {
		avgX = (currentRed[0].x + currentRed[1].x + currentGreen[0].x + currentGreen[1].x) / 4;
		chairSize = 0;
		//find the maximum distance between all points as an approximation to the chair size
		for (int i = 0; i < 2; i++) {
			for int j = 0; j < 2; j++){
				chairSize = max(chairSize, abs(currentRed[i].x - currentGreen[j].x));
			}
		}
		relativeX = avgX - originAvgX;
		int avgRedX = (currentRed[0].x + currentRed[1].x) / 2;
		int avgGreenX = (currentGreen[0].x + currentGreen[1].x) / 2;
		int avgRedY = (currentRed[0].y + currentRed[1].y) / 2;
		int avgGreenY = (currentGreen[0].y + currentGreen[1].y) / 2;

		if (abs(relativeX) > chairSize/10) {
			//if the chair is on the left side and hasn't rotated fully, rotate it to the left
			if (relativeX < 0) {
				if (abs(avgRedY - avgGreenY) < (chairSize / 20) && (avgRedX - avgGreenX) > 0) {
					d = straight;
				}
				else {
					d = left;
					return d;
				}
			}
			//if the chair is on the right side and hasn't rotated fully, rotate it to the right
			else {
				if (abs(avgRedY - avgGreenY) > (chairSize / 20) && (avgRedX - avgGreenX) < 0) {
					d = straight;
				}
				else {
					d = right;
					return d;
				}
			}
		}
		//If chair is in the middle of the camera
		else {
			//If chair facing camera directly
			if (abs(avgRedX - avgGreenX) < chairSize / 10) {
				d = straight;
				return d;
			}
			else if (avgRedX > avgGreenX) {
				d = right;
				return d;
			}
			else {
				d = left;
				return d;
			}

		}
	}

	//return "still" as default
	return d;
}

void sqlStatement(time_t timeIn, int sessionID, char* query){
	char sql[100] = "INSERT INTO information VALUES (";
	char timeBuffer[80];
	struct tm tstruct = *localtime(&timeIn);
	strftime(timeBuffer, sizeof(timeBuffer), "%X", &tstruct);
	char const *tmpSession = to_string(sessionID).c_str();
	strcat(sql,tmpSession);
	strcat(sql, ", '");
	strcat(sql,timeBuffer);
	time_t timeOut = time(0);
	tstruct = *localtime(&timeOut);
	strftime(timeBuffer, sizeof(timeBuffer), "%X", &tstruct);
	strcat(sql, "', '");
	strcat(sql, timeBuffer);
	strcat(sql, "', ");
	int timeDiff = difftime(timeOut,timeIn);
	char const *timeDif = std::to_string(timeDiff).c_str();
	strcat(sql, timeDif);
	strcat(sql, ", '");
	strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d", &tstruct);
	strcat(sql, timeBuffer);
	strcat(sql, "');");
	cout << "SQL statement:\n" << sql << "\n";
	for (int i = 0; i < 100; ++i){
		query[i] = sql[i];
	}
	return;
}

int main(int argc, char **argv) {
	//Database variables setup
	sqlite3 *db;
	char *error;
	time_t timeIn;
	int sessionID = 1;
	//Connect to database, exit on failure
	int rc = sqlite3_open("SmartChair.db", &db);
	if (rc){
		cout << "error opening file";
		sqlite3_close(db);
		return 0;
	}

	//Connect to webcam for live feed input
	Mat origImg;
	VideoCapture cap(0);
	if (!cap.isOpened()){
		cout << "Did not open camera\n";
		return -1;
	}

	//Bluetooth setup
	string command = "Python /home/pi/bluetooth_command.py ";
	command += "argument_for_command";
	system(command.c_str());

	//Center points for origin, used to return to original location
	vector<Point> baseCenterPoints_RED;
	vector<Point> baseCentrePoints_GREEN;

	//Setup origin (and remove noise by reading first few inputs from camera)
	for(int i = 0; i < 50; ++i){
		cap >> origImg;
	}

	//obtain center points for red objects in image
	baseCenterPoints_RED = processImage(origImg,160,100,100,179,255,255);
	baseCentrePoints_GREEN = processImage(origImg,45,100,100,75,255,255);
	
	bool sitting = false;
	while (1) {
		cap >> origImg;

		vector<Point> sittingCenterPoints_YELLOW = processImage(origImg,21,100,100,35,255,255); //
		//Check to see if someone is sitting on chair (missing one centerpoints of contour)
		if (sittingCenterPoints_YELLOW.size() < 1){
			if (!sitting){
				timeIn = time(0);
				sitting = true;
			}
		}
		else{
			if (sitting){
				char sql[100];
				sqlStatement(timeIn, sessionID, sql);
				rc = sqlite3_exec(db,sql,NULL,0,&error);
				if( rc != SQLITE_OK ){
			    	cout << "error executing sqlite3_exec\n";
			    }
			    sessionID++;
			    sitting = false;
			    //user has gotten up
			    //sleep for 2 seconds, then move back back to origin
			    sleep(2);
			    for (int wipe = 0; wipe < 10; ++wipe){
			    	cap >> origImg;
			    }
			    //do processing with respect to base center points and new center points
			    
			    bool notAtOrigin = true;
			    while (notAtOrigin){
			    	cap >> origImg;
			    	vector<Point> newCenterPoints_RED = processImage(origImg,160,100,100,179,255,255);
			    	vector<Point> newCenterPoints_GREEN = processImage(origImg,45,100,100,90,255,255);
			    	//move chair x,y directions to origin (exit if at origin, change notAtOrigin = false)
					Direction dir;
					dir = pathToOrigin(baseCenterPoints_RED, baseCenterPoints_GREEN, newCenterPoints_RED, newCenterPoints_GREEN);
					//repeat till at origin (+/- few pixels of error)
					sendString(dir);
			    }
			    
			}		
		}
	}

	void sendString(Direction dir) {
		string send;

		//SF-255-F-255X
		//S: Start,   F/B: Forward/Backward, 0-255: for power, X: end string

		switch (dir) {
			case straight:
				send = "SF-200-F-200X";
				break;
			case left:
				send = "SB-200-F-200X";
				break;
			case right:
				send = "SF-200-B-200X";
				break;
			default:
				send = "SF-0-F-0X";
				break;
		}
		//NEED TO SEND STRING VIA BLUETOOTH
	}

	sqlite3_close(db);
	return 0;
}