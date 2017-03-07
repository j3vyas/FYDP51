#include "ChairProcessor.h"
#include "ChairFrame.h"
#include "ChairMap.h"
#include "ChairMapping.h"
#include "ChairMove.h"
#include "ChairPath.h"

using namespace std;
    
ChairProcessor::ChairProcessor(ChairFrame baseFrame) {
    this->baseFrame = baseFrame;
    this->mappedBaseFrame = ChairMapping::mapFrame(baseFrame);
	this->basePosition = ChairMapping::mapPosition(mappedBaseFrame);
	this->chairMove = ChairMove();
    this->chairMap = ChairMap(mappedBaseFrame);
	this->chairPath = ChairPath(chairMove, basePosition);
}
    
void ChairProcessor::processCurrentFrame(ChairFrame currentFrame) {
	ChairFrame mappedCurrentFrame;
	ChairPosition currentPosition;

    mappedCurrentFrame = ChairMapping::mapFrame(currentFrame);
	currentPosition = ChairMapping::mapPosition(mappedCurrentFrame);
	chairPath.moveChair(currentPosition);
}