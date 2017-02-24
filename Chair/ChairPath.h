#ifndef CHAIRPATH
#define CHAIRPATH

#include "ChairPosition.h"
#include "ChairFrame.h"
#include "ChairMove.h"

class ChairPath {
	public: 
		ChairPath::ChairPath(ChairMove chairMove);

		void moveChair(ChairPosition currentPos, ChairPosition destination);

	private:
		bool requireStop;

		const double DISTANCE_THRESHOLD = 5;

		ChairMove chairMove;
    
		bool atDestination(ChairPosition currentPos, ChairPosition destination);
    
		bool correctDirection(ChairPosition currentPos, ChairPosition destination, bool turnToDest);
    
		void nextDestination();
};

#endif