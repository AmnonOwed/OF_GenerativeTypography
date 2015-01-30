#pragma once

#include "ofMain.h"

#include "Circle.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void addCircles(int num);

		void keyPressed(int key);

		bool saveOneFrame;

		ofColor bg_color;
		ofColor fbo_color;

		ofPixels pix;

		vector <Circle *> circles;
};
