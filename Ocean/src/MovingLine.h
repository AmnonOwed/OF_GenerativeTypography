#ifndef MOVINGLINE_H
#define MOVINGLINE_H

#include "ofMain.h"

class ofApp;

class MovingLine {
	public:
		MovingLine();

		void setup(vector <ofVec2f> _vertices, ofApp * _ptrOfApp);
		void update();
		void display();

		bool isFullyOutsideBoundaries();

		ofApp * ptrOfApp;
		vector <ofVec2f> vertices;
};

#endif // MOVINGLINE_H
