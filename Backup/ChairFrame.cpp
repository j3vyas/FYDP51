#include "ChairFrame.h"
    
ChairFrame::ChairFrame() {}
    
ChairFrame::ChairFrame(ChairCoord fl, ChairCoord fr, ChairCoord bl, ChairCoord br) {
	this->fl = fl;
	this->fr = fr;
	this->bl = bl;
	this->br = br;
}
    
ChairCoord ChairFrame::getFl() {
	return this->fl;
}
    
void ChairFrame::setFl(ChairCoord fl) {
	this->fl = fl;
}
    
ChairCoord ChairFrame::getFr() {
	return this->fr;
}
    
void ChairFrame::setFr(ChairCoord fr) {
	this->fr = fr;
}
    
ChairCoord ChairFrame::getBl() {
	return this->bl;
}
    
void ChairFrame::setBl(ChairCoord bl) {
	this->bl = bl;
}
    
ChairCoord ChairFrame::getBr() {
	return this->br;
}
    
void ChairFrame::setBr(ChairCoord br) {
	this->br = br;
}