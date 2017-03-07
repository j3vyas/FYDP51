#include "ChairMove.h"

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

ChairMove::ChairMove() {
    //Requrire Bluetooth Connection
}
    
void ChairMove::turnRight() {
	send_motor_command("SF-200-B-200X");
}
    
void ChairMove::turnLeft() {
	send_motor_command("SB-200-F-200X");
}

void ChairMove::moveForward() {
	send_motor_command("SF-200-F-200X");
}
    
void ChairMove::moveStop() {
	send_motor_command("SF-0-F-0X");
}

void ChairMove::send_motor_command(std::string command) {
	string s = "Python bluetooth_test.py ";
	s += command;
	cout << command;
	system(s.c_str());
	return;
}