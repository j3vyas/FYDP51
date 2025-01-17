#include "ChairProcessor.h"
#include "ChairFrame.h"
#include "ChairMap.h"
#include "ChairMapping.h"
#include "ChairMove.h"
#include "ChairPath.h"

using namespace std;

ChairProcessor::ChairProcessor(ChairFrame baseFrameArg):
    baseFrame(baseFrameArg),
    mappedBaseFrame(ChairMapping::mapFrame(baseFrame)),
	basePosition(ChairMapping::mapPosition(mappedBaseFrame)),
	chairMove(ChairMove()),
    chairMap(ChairMap(mappedBaseFrame)),
	chairPath(ChairPath(chairMove, basePosition)),
	mappedPrevFrame(mappedBaseFrame),
	prevPosition(basePosition)
{
}
    
void ChairProcessor::processCurrentFrame(ChairFrame currentFrame) {
    this->mappedCurrentFrame = ChairMapping::frameComparer(ChairMapping::mapFrame(currentFrame), mappedPrevFrame);
	this->currentPosition = ChairMapping::mapPosition(mappedCurrentFrame);
	chairPath.moveChair(this->currentPosition);
	this->mappedPrevFrame = mappedCurrentFrame;
	this->prevPosition = currentPosition;
}

ChairFrame ChairProcessor::getMappedCurrentFrame(){
	return this->mappedCurrentFrame;
}

ChairPosition ChairProcessor::getCurrentPosition() {
	return this->currentPosition;
}