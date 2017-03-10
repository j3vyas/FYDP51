#ifndef CHAIRPATH
#define CHAIRPATH

#include "ChairPosition.h"
#include "ChairFrame.h"
#include "ChairMove.h"

class ChairPath {
	public:
		ChairPath(ChairMove chairMove, ChairPosition basePosition);

		void moveChair(ChairPosition currentPos);

	private:

		const double DISTANCE_THRESHOLD = 5;

		const double RAD_THRESHOLD = 0.0872665; //0.0872665 = 5deg

		const double BUFF_DIST_MULT = 20;

		bool requireStop;

		int chairStage;

		ChairMove chairMove;

		ChairPosition prevPos;

		ChairPosition basePosition;

		ChairPosition bufferPosition;

		void createBufferPosition(ChairPosition basePosition);
    
		bool atDestination(ChairCoord current, ChairCoord dest);
		
		double atDirection(ChairCoord current, ChairCoord dest);

		void moveChairPhase(ChairPosition currentPos);
};

#endif