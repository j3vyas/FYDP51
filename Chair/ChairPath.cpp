#include "ChairPath.h"

using namespace std;


/*  
 *  Create new instance
 *  Pass reference to ChairMove, and Bluetooth
 *  Create base location
 *  Create buffer location
 */

ChairPath::ChairPath(ChairMove chairMove, ChairPosition basePosition) {
	this->chairMove = chairMove;
	this->basePosition = basePosition;
	this->prevPos = basePosition;
	createBufferPosition(basePosition);
	requireStop = true;
	isFirstStage = true;
}

/* 
 *  Sets buffer position before the base position
 */
void ChairPath::createBufferPosition(ChairPosition basePosition) {
	ChairPosition bufferPositionInit = ChairPosition();

	this->bufferPosition = bufferPositionInit;
}

/*  
 *  Inputs current postion
 */
void ChairPath::moveChair(ChairPosition currentPos) {
	//If stop flag on
	if (requireStop) {
		//If not at prevPos, continue stopping, otherwise
		if (atDestination(currentPos.getPosition(),
			prevPos.getPosition())) {
		
			requireStop = false;
		}
		chairMove.moveStop();
    } 
	else {
		moveChairPhase(currentPos);
    }
	prevPos = currentPos;
}

void ChairPath::moveChairPhase(ChairPosition currentPos) {
	if(isFirstStage) {
		//Move to bufferPos
		//If at destination
		if (atDestination(currentPos.getPosition(), 
			bufferPosition.getPosition())) {

			isFirstStage = false;
			requireStop = true;
		}
		//If not at direction, change to direction
		else {
			double diffRad = atDirection(currentPos.getDirection(),
				bufferPosition.getDirection());

			if (abs(diffRad) < RAD_THRESHOLD) {
				//Forward
				chairMove.moveForward();
			} 
			else {
				//Turn to direction
				if (diffRad > 0) {
					//Left
					chairMove.turnLeft();
				}
				else {
					//Right
					chairMove.turnRight();
				}
			}
		}
	}
	else {
		//Move to basePos
		if (atDestination(currentPos.getPosition(), 
			basePosition.getPosition())) {
			//At position, stop
			requireStop = true;
		}
		else {
			double diffRad = atDirection(currentPos.getDirection(),
				basePosition.getDirection());

			if (abs(diffRad) < RAD_THRESHOLD) {
				chairMove.moveForward();
			}
			else {
				//Turn to direction
				if (diffRad > 0) {
					//Left
					chairMove.turnLeft();
				}
				else {
					//Right
					chairMove.turnRight();
				}
			}
		}
	}
}
    
bool ChairPath::atDestination(ChairCoord current, ChairCoord dest) {

	double xdiff = current.x - dest.x;
	double ydiff = current.y - dest.y;

	double distance = sqrt(pow(xdiff, 2) + pow(ydiff, 2));
    
    return (distance < DISTANCE_THRESHOLD);
}

double ChairPath::atDirection(ChairCoord current, ChairCoord dest) {

	double currentRad = atan2(current.y, current.x);
	double destRad = atan2(dest.y, dest.x);

	return destRad - currentRad;
}
