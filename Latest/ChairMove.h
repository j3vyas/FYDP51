#ifndef CHAIRMOVE
#define CHAIRMOVE

#include <string>
#include <Python.h>

class ChairMove {

	public:
		ChairMove();
    
		void turnRight();
    
		void turnLeft();
    
		void moveForward();
    
		void moveStop();

		void closeBTSocket();

private:
	PyObject *pInstance;

	void send_motor_command(std::string command);

};

#endif