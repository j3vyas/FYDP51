#ifndef CHAIRCOORD
#define CHAIRCOORD

#include<string>

struct ChairCoord {
	public:
		double x;
		double y;
		bool missing;
		ChairCoord();
		ChairCoord(double x, double y);
};

#endif