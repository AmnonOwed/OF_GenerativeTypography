#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

		int gradientCounter;

		bool displayStroke;
		bool changeGradientSize;

		ofMesh mesh;

		ofColor fg_color;
		ofColor bg_color;
		ofColor fbo_color;

		ofPixels pix;
};
