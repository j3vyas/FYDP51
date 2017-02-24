#ifndef CHAIRPROCESSOR
#define CHAIRPROCESSOR

#include "ChairMap.h"
#include "ChairFrame.h"

class ChairProcessor {
    
	public: 
		ChairProcessor::ChairProcessor(ChairFrame baseFrame);
		void processCurrentFrame(ChairFrame currentFrame);

	private: 
		ChairFrame baseFrame;
		ChairFrame mappedBaseFrame;
		ChairMap chairMap;

};


#endif