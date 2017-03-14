#include <cv.h>
#include <string>
using namespace std;
using namespace cv;

class object
{
public:
    Object();
    Object(string name);
    
    ~Object(void);
    
    Point getPoint();
    Scalar getHsvMin();
    Scalar getHsvMax();
    Scalar getColourBgr();
    string getColour();
    
    void setPoint(Point point);
    void setHsvMin(Scalar hsv);
    void setHsvMax(Scalar hsv);
    void setColourBgr(Scalar bgr);
    void setColour(string colour);
    
 private:
    Point point;
    Scalar hsvMin, hsvMax, colourBgr;
    string colour;
}