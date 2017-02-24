#include "ChairPath.h"

using namespace std;

ChairPath::ChairPath(ChairMove chairMove) :
	requireStop(true) {
	this->chairMove = chairMove;
}
    
    /*  Inputs destination and current postion
     *  Checks if direction needs to change, if it does, stop
     *
     */
void ChairPath::moveChair(ChairPosition currentPos, ChairPosition destination) {
    if (requireStop) {
        requireStop = false;
        chairMove.moveStop();
    } else {
        if (atDestination(currentPos, destination)) {
            if (correctDirection(currentPos, destination, false)) {
                nextDestination();
            }
			else {
				//Turn to direction
			}
		}
		else {
			if (correctDirection(currentPos, destination, true)) {
				//Move forward
			}
			else {
				//Turn to face the destination
			}
		}
    }
}
    
bool ChairPath::atDestination(ChairPosition currentPos, ChairPosition destination) {

	double xdiff = currentPos.getPosition().x - destination.getPosition().x;
	double ydiff = currentPos.getPosition().y - destination.getPosition().y;

	double distance = sqrt(pow(xdiff, 2) + pow(ydiff, 2));
    
    return (distance < DISTANCE_THRESHOLD);
}
    
bool ChairPath::correctDirection(ChairPosition currentPos, ChairPosition destination, bool turnToDest) {
        
    //If chair is facing correct direction
	if (turnToDest) {

	}
	else {

	}
        
    return true;
}
    
void ChairPath::nextDestination() {
        
}
