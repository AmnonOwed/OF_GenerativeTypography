#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

		bool saveOneFrame;

		ofVec2f center;
		vector <vector <ofVec2f> > points;
		float nextPointSpeed;
		ofPath outline;
};
