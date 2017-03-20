// Sandbox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../Chair/ChairCoord.h"
#include "../../Chair/ChairFrame.h"
#include "../../Chair/ChairProcessor.h"
#include "../../Chair/detection.h"

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
	/*
	double flx, frx, blx, brx, fly, fry, bly, bry;
	string ender;
	cout << "FL.x: ";
	cin >> flx;

	cout << "FL.y: ";
	cin >> fly;

	cout << "FR.x: ";
	cin >> frx;

	cout << "FR.y: ";
	cin >> fry;

	cout << "BL.x: ";
	cin >> blx;

	cout << "BL.y: ";
	cin >> bly;

	cout << "BR.x: ";
	cin >> brx;

	cout << "BR.y: ";
	cin >> bry;
	
	ChairCoord fl = ChairCoord(flx, fly);
	ChairCoord fr = ChairCoord(frx, fry);
	ChairCoord bl = ChairCoord(blx, bly);
	ChairCoord br = ChairCoord(brx, bry);
	
	ChairCoord fl = ChairCoord(1, 2);
	ChairCoord fr = ChairCoord(3, 4);
	ChairCoord bl = ChairCoord(5, 6);
	ChairCoord br = ChairCoord(7, 8);
	ChairFrame chairFrame = ChairFrame(fl, fr, bl, br);

	ChairProcessor chairProcessor = ChairProcessor(chairFrame);

	cout << "Press enter to end";
	cin >> ender;

    return 0;
	*/
	Detection det = Detection();
	det.detectionFunc();
}

