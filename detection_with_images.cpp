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

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
	//Database variables setup
	sqlite3 *db;
	char *error;
	time_t timeIn;
	int sessionID = 1;
	//char *sql = "INSERT INTO information VALUES (1, '17:20:00', '17:45:00', 25, '2016-07-05')";
	int rc = sqlite3_open("SmartChair.db", &db);
	if (rc){
		cout << "error opening file";
		sqlite3_close(db);
		return 0;
	}
	//rc = sqlite3_exec(db,sql,NULL,0,&error);

	VideoCapture cap(0);
	if (!cap.isOpened()){
		cout << "Did not open camera\n";
		return -1;
	}

	bool sitting = false;
	Mat bgr_image;
	while (1) {
		cap >> bgr_image;

		// Show the original images
		namedWindow("Original image", WINDOW_NORMAL);
		imshow("Original image", bgr_image);

		Mat orig_image = bgr_image.clone();
	
		// Convert input image to HSV
		Mat hsv_image;
		cvtColor(bgr_image, hsv_image, COLOR_BGR2HSV);
	
		// Threshold the HSV image
		Mat red_hue_range;
		inRange(hsv_image, Scalar(160, 50, 50), Scalar(255, 50, 50), red_hue_range);
	
		// Combine the above two images
		Mat red_hue_image = red_hue_range.clone();
		imwrite("filteredImg.jpg", red_hue_image);
	
		GaussianBlur(red_hue_image, red_hue_image, Size(5, 5), 2, 2);
	
		// Show the filtered images
		namedWindow("Threshold image", WINDOW_NORMAL);
		imshow("Threshold image", red_hue_image);
		imwrite("cleanedImg.jpg",red_hue_image);
	
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(red_hue_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
		//draw contours
		Mat drawing = Mat::zeros(red_hue_image.size(), CV_8UC3);
		Mat contourDrawing = Mat::zeros(red_hue_image.size(), CV_8UC3);;
		drawContours(contourDrawing, contours, -1, Scalar(255, 255, 255));
		imwrite("contourImg.jpg", contourDrawing);
	
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
			if (isDetected && (contourArea(contours[i]) >= 1000)){
				boundingBox.push_back(rect);
				centrePoints.push_back(rect.center);
			}
		}
		
		//Check to see if someone is sitting on chair (missing one centerpoints of contour)
		if (centrePoints.size() < 5){
			if (!sitting){
				timeIn = time(0);
				sitting = true;
			}
		}
		else{
			if (sitting){
				char sql[100] = "INSERT INTO information VALUES (";
				char timeBuffer[80];
				struct tm tstruct = *localtime(&timeIn);
				strftime(timeBuffer, sizeof(timeBuffer), "%X", &tstruct);
				char const *tmpSession = to_string(sessionID).c_str();
				strcat(sql,tmpSession);
				sessionID++;
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
				sitting = false;
				cout << "SQL statement:\n" << sql << "\n";
			}		
		}
		for (int i = 0; i < centrePoints.size(); ++i){
			cout << centrePoints[i] << " ";
		}
		cout << "\n";
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
		namedWindow("drawing", WINDOW_NORMAL);
		imshow("drawing", drawing);
		imwrite("resultImg.jpg", drawing);
	
		waitKey(200);
	}
	sqlite3_close(db);
	return 0;
}