#ifndef CHAIRMAPPING
#define CHAIRMAPPING

#include "ChairPosition.h"
#include "ChairFrame.h"
#include <string>

class ChairMapping {
    
	public:
		static ChairFrame mapFrame(ChairFrame chairFrame);
		static ChairPosition mapPosition(ChairFrame chairFrame);
		static ChairCoord mapSingleCoord(ChairCoord chairCoord);
		static ChairCoord toCartesian(double radius, double angle);
		static double calculateXCoord(double x);
		static double calculateYCoord(double y);
		static ChairFrame frameComparer(ChairFrame currentFrame, ChairFrame prevFrame);

	private:
		static ChairFrame missingHandler(ChairFrame chairFrame);
		static double calculateDistance(ChairCoord fir, ChairCoord sec);
};

#endif