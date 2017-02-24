#include "ChairProcessor.h"
#include "ChairFrame.h"
#include "ChairMap.h"
#include "ChairMapping.h"

using namespace std;
    
ChairProcessor::ChairProcessor(ChairFrame baseFrame) {
    this->baseFrame = baseFrame;
    this->mappedBaseFrame = ChairMapping::mapFrame(baseFrame);
    this->chairMap = ChairMap(mappedBaseFrame);
}
    
void ChairProcessor::processCurrentFrame(ChairFrame currentFrame) {
    ChairFrame mappedCurrentFrame = ChairFrame();
    mappedCurrentFrame = ChairMapping::mapFrame(currentFrame);
}