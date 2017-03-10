#ifndef CHAIRMAP
#define CHAIRMAP

#include "ChairPosition.h"
#include "ChairFrame.h"

class ChairMap{
	private:
		ChairPosition baseChairPosition;
		ChairPosition currentPosition;
    
	public:
		ChairMap();
    
		ChairMap(ChairFrame baseFrame);
    
		void setBaseChair(ChairFrame baseFrame);
    
		void setCurrentPosition(ChairFrame currentFrame);
    
		ChairPosition convertToPosition(ChairFrame chairFrame);
};

#endif