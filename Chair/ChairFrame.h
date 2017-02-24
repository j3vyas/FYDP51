#ifndef CHAIRFRAME
#define CHAIRFRAME

#include "ChairCoord.h"

class ChairFrame {
	public:
		ChairFrame::ChairFrame();
    
		ChairFrame::ChairFrame(ChairCoord fl, ChairCoord fr, ChairCoord bl, ChairCoord br);
    
		ChairCoord getFl();
    
		void setFl(ChairCoord fl);
    
		ChairCoord getFr();
    
		void setFr(ChairCoord fr);
    
		ChairCoord getBl();
    
		void setBl(ChairCoord bl);
    
		ChairCoord getBr();
    
		void setBr(ChairCoord br);

	private:
		ChairCoord fl, fr, bl, br;
};

#endif