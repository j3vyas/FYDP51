#ifndef CHAIRPROCESSOR
#define CHAIRPROCESSOR

#include "ChairMap.h"
#include "ChairFrame.h"
#include "ChairPosition.h"

class ChairProcessor {
    
	public: 
		ChairProcessor::ChairProcessor(ChairFrame baseFrame);
		void processCurrentFrame(ChairFrame currentFrame);

	private: 
		ChairFrame baseFrame;
		ChairFrame mappedBaseFrame;
		ChairPosition basePosition;
		ChairMap chairMap;
		ChairMove chairMove;
		ChairPath chairPath;

};


#endif