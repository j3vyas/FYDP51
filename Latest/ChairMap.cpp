#include "ChairPosition.h"
#include "ChairMap.h"
#include "ChairMapping.h"

    
ChairMap::ChairMap(){}
    
ChairMap::ChairMap(ChairFrame baseFrame){
    setBaseChair(baseFrame);
}
    
void ChairMap::setBaseChair(ChairFrame baseFrame){
    this->baseChairPosition = convertToPosition(baseFrame);
}
    
void ChairMap::setCurrentPosition(ChairFrame currentFrame){
    this->currentPosition = convertToPosition(currentFrame);
}
    
ChairPosition ChairMap::convertToPosition (ChairFrame chairFrame){
    return ChairMapping::mapPosition(chairFrame);
}
