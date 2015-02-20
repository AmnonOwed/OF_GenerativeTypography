#ifndef MOVINGLINE_H
#define MOVINGLINE_H

#include "ofMain.h"
#include "FlowField.h"

class MovingLine {
	public:
		MovingLine();

		void setup(vector <ofVec2f> _vertices);
		void update();
		void display(float minX, float maxX);

		bool isFullyOutsideBoundaries(float minimum);

		vector <ofVec2f> vertices;
};

#endif // MOVINGLINE_H
