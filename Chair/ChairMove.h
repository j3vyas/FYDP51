#ifndef CHAIRMOVE
#define CHAIRMOVE

#include <string>

class ChairMove {
	public:
		ChairMove::ChairMove();
    
		void turnRight();
    
		void turnLeft();
    
		void moveForward();
    
		void moveStop();

	private:
		static const int BASE_SPEED = 1;	//Change this to change base speed

		void send_motor_command(std::string command);
};

#endif