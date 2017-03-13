#include "ChairPosition.h"
#include "ChairFrame.h"
#include "ChairMap.h"
#include "ChairMapping.h"
#include "ChairCoord.h"


#define M_PI 3.14159265358979323846  /* pi */
#define HEIGHT 30.0
#define HEIGHT_RAD 0.7505
#define X_PIXEL_RAD 0.001304
#define X_CENTER 320.0
#define Y_PIXEL_RAD 0.0013378
#define Y_TOTAL 480.0

#include <string>
#include <iostream>
#include <cmath>

using namespace std;

/*
 *  Convert image coordinates to map coordinates
 */
ChairFrame ChairMapping::mapFrame(ChairFrame chairFrame) {
    ChairFrame mappedFrame = ChairFrame();
    
	chairFrame = missingHandler(chairFrame);

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

ChairFrame ChairMapping::missingHandler(ChairFrame chairFrame) {
	int legsMissing = 0;
	ChairCoord chairCoords[] = { 
		chairFrame.getBl(),
		chairFrame.getBr(),
		chairFrame.getFl(),
		chairFrame.getFr()
	};
	for (int i = 0; i < 4; i++) {
		if (chairCoords[i].missing) {
			legsMissing++;
		}
	}

	if (legsMissing == 0) {
		return chairFrame;
	}
	else if (legsMissing == 1) {
		if (chairCoords[0].missing) {
			chairFrame.setBl(ChairCoord(
				chairFrame.getBr().x + chairFrame.getFl().x - chairFrame.getFr().x,
				chairFrame.getBr().y + chairFrame.getFl().y - chairFrame.getFr().y
			));
		}
		else if (chairCoords[1].missing) {
			chairFrame.setBr(ChairCoord(
				chairFrame.getBl().x + chairFrame.getFr().x - chairFrame.getFl().x,
				chairFrame.getBl().y + chairFrame.getFr().y - chairFrame.getFl().y
			));
		}
		else if (chairCoords[2].missing) {
			chairFrame.setBl(ChairCoord(
				chairFrame.getFr().x + chairFrame.getBl().x - chairFrame.getBr().x,
				chairFrame.getFr().y + chairFrame.getBl().y - chairFrame.getBr().y
			));
		}
		else if (chairCoords[3].missing) {
			chairFrame.setFr(ChairCoord(
				chairFrame.getFl().x + chairFrame.getBr().x - chairFrame.getBl().x,
				chairFrame.getFl().y + chairFrame.getBr().y - chairFrame.getBl().y
			));
		}
	}
	else if (legsMissing == 2) {
		double xTemp = 0;
		double yTemp = 0;
		for (int i = 0; i < 4; i++) {
			xTemp += chairCoords[i].x;
			yTemp += chairCoords[i].y;
		}

		for (int i = 0; i < 4; i++) {
			if (chairCoords[i].missing) {
				if (i == 0) {
					chairFrame.setBl(ChairCoord(xTemp/2, yTemp/2));
				}
				else if (i == 1) {
					chairFrame.setBr(ChairCoord(xTemp/2, yTemp/2));
				}
				else if (i == 2) {
					chairFrame.setFl(ChairCoord(xTemp/2, yTemp/2));
				}
				else {
					chairFrame.setFr(ChairCoord(xTemp/2, yTemp/2));
				}
			}
		}
	}

	return chairFrame;
}

/* 
 *  Convert map coordinates to single coordinate vector
 */
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
