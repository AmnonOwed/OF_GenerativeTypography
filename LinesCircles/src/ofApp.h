#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void resetPointsAndColors();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

		bool saveOneFrame;
		bool bResetPointsAndColors;

		int numPoints;
		float maxDistance;

		ofColor line_color;
		ofColor bg_color;
		ofColor fbo_color;

		ofPixels pix;

		vector <ofVec3f> points; // list to store all the points (x, y, radius)
		vector <ofColor> colors; // list to store all the colors
};
