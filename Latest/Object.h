#ifndef OBJECT
#define OBJECT
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Object
{
	public:
    		Object();
    		Object(string name);
    
    		~Object(void);
    
    		Point getPoint();
    		Scalar getHsvMin();
    		Scalar getHsvMax();
		Scalar getHsvMinUpper();
		Scalar getHsvMaxUpper();
    		Scalar getColourBgr();
    		string getColour();
    
    		void setPoint(Point point);
    		void setHsvMin(Scalar hsv);
    		void setHsvMax(Scalar hsv);
		void setHsvMinUpper(Scalar hsv);
		void setHsvMaxUpper(Scalar hsv);
    		void setColourBgr(Scalar bgr);
    		void setColour(string colour);
    
 private:
    		Point point;
    		Scalar hsvMin, hsvMax, hsvMinUpper, hsvMaxUpper, colourBgr;
    		string colour;
};
#endif