#pragma once

#include "ofMain.h"

#include "Rd.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		ofColor fg_color;
		ofColor bg_color;
		ofColor fbo_color;

		ofImage result;

		Rd rd;
};
