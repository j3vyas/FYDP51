#include "ChairPosition.h"
#include "ChairCoord.h"

ChairPosition::ChairPosition() {
}
    
ChairPosition::ChairPosition(ChairCoord position, ChairCoord direction) {
    setPosition(position);
    setDirection(direction);
}
    
void ChairPosition::setPosition(ChairCoord position) {
    this->position = position;
}
    
void ChairPosition::setDirection(ChairCoord direction) {
    this->direction = direction;
}
    
ChairCoord ChairPosition::getPosition() {
    return this->position;
}
    
ChairCoord ChairPosition::getDirection() {
    return this->direction;
}
