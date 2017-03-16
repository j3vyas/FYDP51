#include "ChairPath.h"
#include <cmath>

using namespace std;


/*  
 *  Create new instance
 *  Pass reference to ChairMove, and Bluetooth
 *  Create base location
 *  Create buffer location
 */
ChairPath::ChairPath(ChairMove chairMove, ChairPosition basePosition):
	chairMove(chairMove),
	basePosition(basePosition),
	prevPos(basePosition),
	requireStop(true),
	chairStage(0)
{
	createBufferPosition(basePosition);
}

/* 
 *  Sets buffer position before the base position
 */
void ChairPath::createBufferPosition(ChairPosition basePosition) {
	ChairPosition bufferPositionInit = ChairPosition();
	double dirX = basePosition.getDirection().x;
	double dirY = basePosition.getDirection().y;

	double a = sqrt(1 / (dirX*dirX + dirY*dirY));

	dirX = dirX*a;
	dirY = dirY*a;

	bufferPositionInit.setPosition(
		ChairCoord(basePosition.getPosition().x - dirX*BUFF_DIST_MULT,
			basePosition.getPosition().y - dirY*BUFF_DIST_MULT)
	);

	bufferPositionInit.setDirection(ChairCoord(dirX, dirY));

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
	if(chairStage == 1) {
		//Move to bufferPos
		//If at destination
		if (atDestination(currentPos.getPosition(), 
			bufferPosition.getPosition())) {

			chairStage = 2;
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
	else if (chairStage == 2) {
		//Move to basePos
		if (atDestination(currentPos.getPosition(), 
			basePosition.getPosition())) {
			//At position, stop
			requireStop = true;
			chairStage = 0;
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
	else if (chairStage == 0) {
		if (!atDestination(currentPos.getPosition(),
			basePosition.getPosition())) {
			chairStage = 1;
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
