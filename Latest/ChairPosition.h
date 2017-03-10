#ifndef CHAIRPOSITION
#define CHAIRPOSITION

#include "ChairCoord.h"

class ChairPosition {
	public:
		ChairPosition();
    
		ChairPosition(ChairCoord position, ChairCoord direction);
    
		void setPosition(ChairCoord position);
    
		void setDirection(ChairCoord direction);
    
		ChairCoord getPosition();
    
		ChairCoord getDirection();

	private:
		ChairCoord position;
		ChairCoord direction;
};

#endif