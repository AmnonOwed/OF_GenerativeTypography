#ifndef MOVINGLINE_H
#define MOVINGLINE_H

#include "ofMain.h"

class MovingLine {
	public:
		MovingLine();

		void setup(vector <ofVec2f> _vertices);
		void update();
		void display(float minX, float maxX);

		bool isFullyOutsideBoundaries(float minimum);
		ofVec2f getVelocity(const ofVec2f & pos);

		vector <ofVec2f> vertices;
};

#endif // MOVINGLINE_H
