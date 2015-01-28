#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void mousePressed(int x, int y, int button);

		ofColor bg_color;

		ofFbo fbo;
};
