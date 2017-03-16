#include "Object.h"

Object::Object(){
}

Object::Object(string name){
    setColour(name);
    if (name == "red"){
        setColourBgr(Scalar(0, 0, 255));
        setHsvMin(Scalar(0, 40, 50));
        setHsvMax(Scalar(10, 180, 170));
        setHsvMinUpper(Scalar(170, 40, 50));
        setHsvMaxUpper(Scalar(179, 180, 170));
    }

    if (name == "blue"){
        setColourBgr(Scalar(255, 0, 0));
        setHsvMin(Scalar(145, 40, 40));
        setHsvMax(Scalar(170, 140, 170));
    }

    if (name == "green"){
        setColourBgr(Scalar(0, 255, 0));
        setHsvMin(Scalar(50, 60, 50));
        setHsvMax(Scalar(70, 150, 170));
    }

    if (name == "yellow"){
        setColourBgr(Scalar(0, 255, 255));
        setHsvMin(Scalar(70, 40, 40));
        setHsvMax(Scalar(100, 150, 170));
    }    
}

Object::~Object(void){
}

Point Object::getPoint(){
    return Object::point;
}

Scalar Object::getHsvMin(){
    return Object::hsvMin;
}

Scalar Object::getHsvMax(){
    return Object::hsvMax;
}

Scalar Object::getHsvMinUpper(){
    return Object::hsvMinUpper;
}

Scalar Object::getHsvMaxUpper(){
    return Object::hsvMaxUpper;
}

Scalar Object::getColourBgr(){
    return Object::colourBgr;
}

string Object::getColour(){
    return Object::colour;
}

void Object::setPoint(Point point){
    Object::point = point;
}

void Object::setHsvMin(Scalar hsv){
    Object::hsvMin = hsv;
}

void Object::setHsvMax(Scalar hsv){
    Object::hsvMax = hsv;
}

void Object::setHsvMinUpper(Scalar hsv){
    Object::hsvMinUpper = hsv;
}

void Object::setHsvMaxUpper(Scalar hsv){
    Object::hsvMaxUpper = hsv;
}

void Object::setColourBgr(Scalar colour){
    Object::colourBgr = colour;
}

void Object::setColour(string colour){
    Object::colour = colour;
}

