#pragma once

#include "ofMain.h"

#include "Particle.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void updateDrawMode();
		void resetParticles();

		void mousePressed(int x, int y, int button);

		int maxParticles;
		float globalRotation;

		ofColor bg_color;
		ofColor fbo_color;

		bool bUpdateDrawMode;
		bool bResetParticles;

		ofFbo fbo;
		ofPixels pix;

		vector <Particle *> particles;
};
