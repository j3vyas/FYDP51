#include "ChairPosition.h"
#include "ChairFrame.h"
#include "ChairMap.h"
#include "ChairMapping.h"
#include "ChairCoord.h"


#define M_PI 3.14159265358979323846  /* pi */
#define HEIGHT 23.0
#define HEIGHT_RAD 0.7505
#define X_PIXEL_RAD 0.001304
#define X_CENTER 320.0
#define Y_PIXEL_RAD 0.0013378
#define Y_TOTAL 480.0

#include <string>
#include <iostream>

using namespace std;
    
ChairFrame ChairMapping::mapFrame(ChairFrame chairFrame) {
    ChairFrame mappedFrame = ChairFrame();
        
	mappedFrame.setBl(mapSingleCoord(chairFrame.getBl()));
	mappedFrame.setBr(mapSingleCoord(chairFrame.getBr()));
	mappedFrame.setFr(mapSingleCoord(chairFrame.getFr()));
	mappedFrame.setFl(mapSingleCoord(chairFrame.getFl()));

	string chairMapString = "";

	chairMapString = "FL:[" + std::to_string(mappedFrame.getFl().x) + "," + std::to_string(mappedFrame.getFl().y) + "]"
		+ "FR:[" + std::to_string(mappedFrame.getFr().x) + "," + std::to_string(mappedFrame.getFr().y) + "]"
		+ "BL:[" + std::to_string(mappedFrame.getBl().x) + "," + std::to_string(mappedFrame.getBl().y) + "]"
		+ "BR:[" + std::to_string(mappedFrame.getBr().x) + "," + std::to_string(mappedFrame.getBr().y) + "]";

	cout << chairMapString << endl;
    return mappedFrame;
}
    
ChairPosition ChairMapping::mapPosition(ChairFrame chairFrame) {
    ChairPosition chairPosition = ChairPosition();
        
	double positionX = 0.25 * (chairFrame.getFl().x + chairFrame.getBl().x + chairFrame.getBr().x + chairFrame.getFr().x);
	double positionY = 0.25 * (chairFrame.getFl().y + chairFrame.getBl().y + chairFrame.getBr().y + chairFrame.getFr().y);

	double directionX = 0.5*(chairFrame.getFl().x + chairFrame.getFr().x - (chairFrame.getBl().x + chairFrame.getBr().x));
	double directionY = 0.5*(chairFrame.getFl().y + chairFrame.getFr().y - (chairFrame.getBl().y + chairFrame.getBr().y));

	chairPosition.setPosition(ChairCoord(positionX, positionY));
	chairPosition.setPosition(ChairCoord(directionX, directionY));

	string chairPosString = "";

	chairPosString = "POS:[" + std::to_string(positionX) + "," + std::to_string(positionY) + "]"
		+ "DIR:[" + std::to_string(directionX) + "," + std::to_string(directionY) + "]";

	cout << chairPosString << endl;

    return chairPosition;
}

ChairCoord ChairMapping::mapSingleCoord(ChairCoord chairCoord) {
	double x_rad = calculateXCoord(chairCoord.x);
	double y_rad = calculateYCoord(chairCoord.y);

	double radius = HEIGHT * std::tan(y_rad + HEIGHT_RAD);
	double angle = M_PI/2 + x_rad;

	return toCartesian(radius, angle);
}

ChairCoord ChairMapping::toCartesian(double radius, double angle) {
	ChairCoord chairCoord = ChairCoord();
	chairCoord.x = radius * std::cos(angle);
	chairCoord.y = radius * std::sin(angle);
	return chairCoord;
}

double ChairMapping::calculateXCoord(double x) {
	return X_PIXEL_RAD * (x - X_CENTER);
}

double ChairMapping::calculateYCoord(double y) {
	return Y_PIXEL_RAD * (Y_TOTAL - y);
}
