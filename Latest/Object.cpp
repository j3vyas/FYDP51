#include "Object.h"

Object::Object(){
}

Object::Object(string name){
    setColour(name);
    if (name == "red"){
        setColourBgr(Scalar(0, 0, 255));
        setHsvMin(Scalar(160, 100, 40));
        setHsvMax(Scalar(179, 255, 255));
    }

    if (name == "blue"){
        setColourBgr(Scalar(255, 0, 0));
        setHsvMin(Scalar(115, 95, 95));
        setHsvMax(Scalar(135, 160, 165));
    }

    if (name == "green"){
        setColourBgr(Scalar(0, 255, 0));
        setHsvMin(Scalar(70, 60, 60));
        setHsvMax(Scalar(90, 160, 160));
    }

    if (name == "yellow"){
        setColourBgr(Scalar(0, 255, 255));
        setHsvMin(Scalar(20, 70, 50));
        setHsvMax(Scalar(40, 135, 150));
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

void Object::setColourBgr(Scalar colour){
    Object::colourBgr = colour;
}

void Object::setColour(string colour){
    Object::colour = colour;
}

