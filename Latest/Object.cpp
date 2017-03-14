#include "Object.h"

Object::Object(){
}

Object::Object(string name){
    setColour(name);
    if (name == "red"){
        setColour(Scalar(0, 0, 255));
        setHsvMin(Scalar(160, 80, 20));
        setHsvMax(Scalar(179, 120, 140));
    }

    if (name == "blue"){
        setColour(Scalar(255, 0, 0));
        setHsvMin(Scalar(110, 20, 40));
        setHsvMax(Scalar(140, 70, 90));
    }

    if (name == "green"){
        setColour(Scalar(0, 0, 255));
        setHsvMin(Scalar(50, 60, 40));
        setHsvMax(Scalar(80, 190, 90));
    }

    if (name == "yellow"){
        setColour(Scalar(0, 0, 255));
        setHsvMin(Scalar(10, 100, 40));
        setHsvMax(Scalar(30, 255, 100));
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

