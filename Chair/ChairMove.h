#ifndef CHAIRMOVE
#define CHAIRMOVE

class ChairMove {
	public:
		ChairMove::ChairMove();
    
		void turnRight();
    
		void turnLeft();
    
		void moveForward();
    
		void moveBack();
    
		void moveStop();
    
		void moveAll(int allWheels);
    
		void moveSide(int leftWheels, int rightWheels);
    
		void moveIndividual(int leftFront, int leftBack, int rightFront, int rightBack);

	private:
		static const int BASE_SPEED = 1;	//Change this to change base speed
};

#endif