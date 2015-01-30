#ifndef CIRCLE_H
#define CIRCLE_H

#include "ofMain.h"

class Circle {
	public:
		Circle();

		void setup(int _x, int _y);
		void update();
		void display();

		void overlap(const vector <Circle *> circles);

		int x;
		int y;

		float radius;
		float maxRadius;
		float growRate;

		bool isMaxRadius;
		bool isOverlap;

		ofColor c;
};

#endif // CIRCLE_H
