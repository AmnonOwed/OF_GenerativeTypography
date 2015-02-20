#pragma once

#include "ofMain.h"

#include "MovingLine.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void reset();
		ofVec2f getVelocity(const ofVec2f & pos);
		void keyPressed(int key);

		bool saveOneFrame;
		bool bReset;

		vector <vector <ofVec2f> > points;
		vector <vector <ofVec2f> > pointsCopy;
		vector <MovingLine> lines;

		float minX;
		float maxX;
		float minY;
		float maxY;
};
