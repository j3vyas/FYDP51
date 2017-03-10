#ifndef CHAIRPROCESSOR
#define CHAIRPROCESSOR

#include "ChairMap.h"
#include "ChairFrame.h"
#include "ChairPosition.h"
#include "ChairMove.h"
#include "ChairPath.h"

class ChairProcessor {
    
	public: 
		ChairProcessor(ChairFrame baseFrameArg);
		void processCurrentFrame(ChairFrame currentFrame);
		ChairFrame getMappedCurrentFrame();
	private: 
		ChairFrame baseFrame;
		ChairFrame mappedBaseFrame;
		ChairFrame mappedCurrentFrame;
		ChairPosition basePosition;
		ChairMove chairMove;
		ChairMap chairMap;
		ChairPath chairPath;

};


#endif