#include "ChairMove.h"

#include <string>

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

void send_motor_command(string command) {
	string s = "Python bluetooth_test.py ";
	s += command;
	system(s.c_str());
	return;
}