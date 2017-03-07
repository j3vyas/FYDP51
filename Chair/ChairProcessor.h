#ifndef CHAIRPROCESSOR
#define CHAIRPROCESSOR

#include "ChairMap.h"
#include "ChairFrame.h"
#include "ChairPosition.h"
#include "ChairMove.h"
#include "ChairPath.h"

class ChairProcessor {
    
	public: 
		ChairProcessor::ChairProcessor(ChairFrame baseFrameArg);
		void processCurrentFrame(ChairFrame currentFrame);

	private: 
		ChairFrame baseFrame;
		ChairFrame mappedBaseFrame;
		ChairPosition basePosition;
		ChairMove chairMove;
		ChairMap chairMap;
		ChairPath chairPath;

};


#endif