#include "ChairMove.h"
    
ChairMove::ChairMove() {
    //Requrire Bluetooth Connection
}
    
void ChairMove::turnRight() {
    moveSide(BASE_SPEED, -BASE_SPEED);
}
    
void ChairMove::turnLeft() {
    moveSide(-BASE_SPEED, BASE_SPEED);
}

void ChairMove::moveForward() {
    moveAll(BASE_SPEED);
}
    
void ChairMove::moveBack() {
    moveAll(-BASE_SPEED);
}
    
void ChairMove::moveStop() {
    moveAll(0);
}
    
void ChairMove::moveAll(int allWheels) {
    moveSide(allWheels, allWheels);
}
    
void ChairMove::moveSide(int leftWheels, int rightWheels) {
    moveIndividual(leftWheels, leftWheels, rightWheels, rightWheels);
}
    
void ChairMove::moveIndividual(int leftFront, int leftBack, int rightFront, int rightBack) {
    //Call some function to relay via Bluetooth
}