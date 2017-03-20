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
using namespace cv;
using namespace std;

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

void drawObjects(vector<RotatedRect> boundingBox, vector<Point> centrePoints, Mat &drawMat, Scalar colour) {
    // draw the rotated rect and centre point
    for (int i = 0; i < boundingBox.size(); i++) {
        Point2f corners[4];
        boundingBox[i].points(corners);
        line(drawMat, corners[0], corners[1], colour);
        line(drawMat, corners[1], corners[2], colour);
        line(drawMat, corners[2], corners[3], colour);
        line(drawMat, corners[3], corners[0], colour);
        circle(drawMat, centrePoints[i], 3, colour, -1);
    }
}

bool isRectangle(vector<Point> contour) {
    double perim = arcLength(contour, true);
    vector<Point> approx;
    approxPolyDP(contour, approx, 0.06 * perim, true);
    if(approx.size() == 4) {
        Rect rect = boundingRect(approx);
        if(rect.height > rect.width)
            return true;
    }

    return false;
}

void trackObjects(Mat &filteredImg, Mat &drawMat, Object &obj) {
    vector<vector<Point> > contours;
    findContours(filteredImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    vector<RotatedRect> boundingBox;
    vector<Point> centrePoints;
    vector<RotatedRect> maxBox;
    vector<Point> maxPoint;
    double maxArea = 0;
    for (int i = 0; i < contours.size(); i++) {
        RotatedRect rect = minAreaRect(contours[i]);

        if (contourArea(contours[i]) >= 300) {
            boundingBox.push_back(rect);
            centrePoints.push_back(rect.center);
            if (isRectangle(contours[i]) && contourArea(contours[i]) > maxArea) {
                maxBox.clear();
                maxBox.push_back(rect);
                maxPoint.clear();
                maxPoint.push_back(rect.center);
                maxArea = contourArea(contours[i]);
                obj.setPoint(rect.center);
            }
        }
    }
    drawObjects(maxBox, maxPoint, drawMat, obj.getColourBgr());
    //drawObjects(boundingBox, centrePoints, drawMat, obj.getColourBgr());
}

void setHsvRange(Object &obj) {
    if(obj.getColour() == "green") {
        obj.setHsvMin(Scalar(greenLowH, greenLowS, greenLowV));
        obj.setHsvMax(Scalar(greenHighH, greenHighS, greenHighV));
    }
    else if(obj.getColour() == "blue") {
        obj.setHsvMin(Scalar(blueLowH, blueLowS, blueLowV));
        obj.setHsvMax(Scalar(blueHighH, blueHighS, blueHighV));
    }
    else if(obj.getColour() == "red") {
        obj.setHsvMin(Scalar(redLowH, redLowS, redLowV));
        obj.setHsvMax(Scalar(redHighH, redHighS, redHighV));
	obj.setHsvMinUpper(Scalar(redLowHUpper, redLowSUpper, redLowVUpper));
        obj.setHsvMaxUpper(Scalar(redHighHUpper, redHighSUpper, redHighVUpper));
    }
    else {
        obj.setHsvMin(Scalar(yellowLowH, yellowLowS, yellowLowV));
        obj.setHsvMax(Scalar(yellowHighH, yellowHighS, yellowHighV));
    }
}

void morphImg(Mat &img) {
    // use erosion and dilation to remove noise
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(10, 10));

    erode(img, img, erodeElement);
    erode(img, img, erodeElement);

    dilate(img, img, dilateElement);
    dilate(img, img, dilateElement);
}

void getObjects(Mat hsvImg, Object &obj, Mat &points) {
    // threshold matrix
    Mat threshold;

    // refresh the threshold range
    setHsvRange(obj);

    // threshold the image with hsv range
    inRange(hsvImg, obj.getHsvMin(), obj.getHsvMax(), threshold);
	
	//extend the range if the colour is red
	if(obj.getColour() == "red"){
		Mat threshold2;
		inRange(hsvImg, obj.getHsvMinUpper(), obj.getHsvMaxUpper(), threshold2);
		addWeighted(threshold, 1, threshold2, 1, 0, threshold);
	}

    // use morphological transformations to further remove noise
    morphImg(threshold);

    // initialize the point to (-1, -1)
    obj.setPoint(Point(-1, -1));

    // finally track the objects
    trackObjects(threshold, points, obj);
}

bool readCapture(VideoCapture cap, Mat img) {
	cap.read(img);
	cap.read(img);
	cap.read(img);
	cap.read(img);
	cap.read(img);
    bool success = cap.read(img);
    if (!success) {
        cout << "read unsuccessful" << endl;
    }
    return success;
}

ChairFrame getChairFrame(Point p1, Point p2, Point p3, Point p4) {
    ChairCoord fl = ChairCoord(p1.x, p1.y);
	if(p1.x == 0 && p1.y == 0) {
		fl.missing = true;
	}
    ChairCoord fr = ChairCoord(p2.x, p2.y);

	if(p2.x == 0 && p2.y == 0) {
		fr.missing = true;
	}
    ChairCoord bl = ChairCoord(p3.x, p3.y);
	if(p3.x == 0 && p3.y == 0) {
		bl.missing = true;
	}
    ChairCoord br = ChairCoord(p4.x, p4.y);
	if(p4.x == 0 && p4.y == 0) {
		br.missing = true;
	}
    return ChairFrame(fl, fr, bl, br);
}

int getNumPointsFound(Object &blue, Object &green, Object &yellow, Object &red) {
    int numPointsFound = 0;
    Point p1 = blue.getPoint();
    Point p2 = green.getPoint();
    Point p3 = yellow.getPoint();
    Point p4 = red.getPoint();
    if (p1.x != -1 && p1.y != -1) {
        numPointsFound++;
    }
    else {
        p1.x = 0;
        p1.y = 0;
    }
    if (p2.x != -1 && p2.y != -1) {
        numPointsFound++;
    }
    else {
        p2.x = 0;
        p2.y = 0;
    }
    if (p3.x != -1 && p3.y != -1) {
        numPointsFound++;
    }
    else {
        p3.x = 0;
        p3.y = 0;
    }
    if (p4.x != -1 && p4.y != -1) {
        numPointsFound++;
    }
    else {
        p4.x = 0;
        p4.y = 0;
    }
    blue.setPoint(p1);
    green.setPoint(p2);
    yellow.setPoint(p3);
    red.setPoint(p4);
    return numPointsFound;
}

void drawMappedPoints(Mat &map, ChairProcessor cp) {
	ChairFrame cf = cp.getMappedCurrentFrame();    
	ChairCoord fl = cf.getFl();
    ChairCoord fr = cf.getFr();
    ChairCoord bl = cf.getBl();
    ChairCoord br = cf.getBr();
	ChairCoord position = cp.getCurrentPosition().getPosition();
	ChairCoord direction = cp.getCurrentPosition().getDirection();
	direction = ChairCoord(position.x + direction.x, position.y + direction.y);
    cout << "mapped points:" << endl;
    cout << "(" << fl.x << "," << fl.y << ") " << "(" << fr.x << "," << fr.y << ") " << "(" << bl.x << "," << bl.y << ") " << "(" << br.x << "," << br.y << ")" << endl;
    Point p1 = Point(map.rows / 2 + fl.x * 3, map.cols - fl.y * 3);
    Point p2 = Point(map.rows / 2 + fr.x * 3, map.cols - fr.y * 3);
    Point p3 = Point(map.rows / 2 + bl.x * 3, map.cols - bl.y * 3);
    Point p4 = Point(map.rows / 2 + br.x * 3, map.cols - br.y * 3);
	cout << "position x, y's are: " << "(" << position.x << ", " << position.y << ")" << endl;
	Point p5 = Point(map.rows / 2 + position.x * 3, map.cols - position.y * 3);
	cout << "direction x, y's are: " << "(" << direction.x << ", " << direction.y << ")" << endl;
	Point p6 = Point(map.rows / 2 + direction.x * 3, map.cols - direction.y * 3);
    circle(map, p1, 3, Scalar(255, 0, 0), -1);
    circle(map, p2, 3, Scalar(0, 255, 0), -1);
    circle(map, p3, 3, Scalar(0, 255, 255), -1);
    circle(map, p4, 3, Scalar(0, 0, 255), -1);
	circle(map, p5, 3, Scalar::all(255), -1);
	circle(map, p6, 3, Scalar::all(255), -1);
    putText(map, "fl", Point(p1.x, p1.y - 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1, 8);
    putText(map, "fr", Point(p2.x, p2.y - 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 1, 8);
    putText(map, "bl", Point(p3.x, p3.y - 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 1, 8);
    putText(map, "br", Point(p4.x, p4.y - 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 1, 8);
	putText(map, "pos", Point(p5.x, p5.y - 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255), 1, 8);
	putText(map, "dir", Point(p6.x, p6.y - 40), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255), 1, 8);
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

void createControls(Object blue, Object green, Object yellow, Object red) {

    //initialize control variables
    Scalar redHsvMin = red.getHsvMin();
    Scalar redHsvMax = red.getHsvMax();
    redLowH = redHsvMin[0];
    redHighH = redHsvMax[0];
    redLowS = redHsvMin[1];
    redHighS = redHsvMax[1];
    redLowV = redHsvMin[2];
    redHighV = redHsvMax[2];

	Scalar redHsvMinUpper = red.getHsvMinUpper();
	Scalar redHsvMaxUpper = red.getHsvMaxUpper();
    redLowHUpper = redHsvMinUpper[0];
    redHighHUpper = redHsvMaxUpper[0];
    redLowSUpper = redHsvMinUpper[1];
    redHighSUpper = redHsvMaxUpper[1];
    redLowVUpper = redHsvMinUpper[2];
    redHighVUpper = redHsvMaxUpper[2];

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

	    namedWindow("redUpper", CV_WINDOW_AUTOSIZE);
    createTrackbar("LowH", "redUpper", &redLowHUpper, 179);
    createTrackbar("HighH", "redUpper", &redHighHUpper, 179);
    createTrackbar("LowS", "redUpper", &redLowSUpper, 255);
    createTrackbar("HighS", "redUpper", &redHighSUpper, 255);
    createTrackbar("LowV", "redUpper", &redLowVUpper, 255);
    createTrackbar("HighV", "redUpper", &redHighVUpper, 255);

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
    //Testing DB stuff
	char sql[100];
	timeIn = time(0);
	sqlStatement(timeIn, sessionID, sql);
    	rc = sqlite3_exec(db,sql,NULL,0,&error);
	if( rc != SQLITE_OK ){
		cout << "error executing sqlite3_exec\n";
	}

    // exit if capture wasn't successful
    // try capturing image from the webcam
    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	cap.set(CV_CAP_PROP_BUFFERSIZE, 1);
    Mat origImg;
    Mat hsvImg;
    Object blue;
    Object green;
    Object yellow;
    Object red;
    Mat points;
    bool firstRun = true;

    while(true) {
        if (!cap.isOpened()) {
            cout << "webcam not on" << endl;
            return -1;
        }

        cout << "Capturing base frame" << endl;

        // find the base frame with the initial image processing
        cap.read(origImg);
	cap.read(origImg);
	cap.read(origImg);
	cap.read(origImg);
	cap.read(origImg);
	cap.read(origImg);
	
        imshow("base unprocessed", origImg);

        // blur the image to get rid of noise
        medianBlur(origImg, origImg, 19);

        // convert input image to HSV
        cvtColor(origImg, hsvImg, COLOR_BGR2HSV);

        // get the base frame
        blue = Object("blue");
        green = Object("green");
        yellow = Object("yellow");
        red = Object("red");

        // create control windows
        if(firstRun) {
            createControls(blue, green, yellow, red);
            firstRun = false;
        }

        // matrix to draw points on
        points = Mat::zeros(hsvImg.size(), CV_8UC3);

        // get objects with the corresponding colour
        thread t1(getObjects, hsvImg, ref(blue), ref(points));
        thread t2(getObjects, hsvImg, ref(green), ref(points));
        thread t3(getObjects, hsvImg, ref(yellow), ref(points));
        thread t4(getObjects, hsvImg, ref(red), ref(points));

        t1.join();
        t2.join();
        t3.join();
        t4.join();
	
        imshow("base", points);


	waitKey(100);
        string word;
        while (word != "start" && word != "r") {
            cout << "Type \"r\" to recapture the base frame or \"start\" to start" << endl;
            getline(cin, word);
        }
        if(word.compare("start") == 0) {
            break;
        }
    }
    ChairFrame baseFrame = getChairFrame(blue.getPoint(), green.getPoint(), yellow.getPoint(), red.getPoint());
    ChairProcessor cp = ChairProcessor(baseFrame);

    // capture loop
    while (true) {

	timeIn = time(0);
	sqlStatement(timeIn, sessionID, sql);
    	rc = sqlite3_exec(db,sql,NULL,0,&error);
	if( rc != SQLITE_OK ){
		cout << "error executing sqlite3_exec\n";
	}

        //break the loop if capture isn't successful
        if (!readCapture(cap, origImg)) {
            break;
        }

        medianBlur(origImg, origImg, 19);
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
        if (numPointsFound >= 2) {
            ChairFrame cf = getChairFrame(blue.getPoint(), green.getPoint(), yellow.getPoint(), red.getPoint());
            cp.processCurrentFrame(cf);
            Mat mappedPoints(600, 600, CV_8UC3, Scalar(0,0,0));
            drawMappedPoints(mappedPoints, cp);
            imshow("mappedPoints", mappedPoints);
        }
	
	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	
        // exit with esc key
        int key = waitKey(30);
        cout << "key entered is " << key << endl;
        if (key == 27) {
            break;
        }
    }
    return 0;
}