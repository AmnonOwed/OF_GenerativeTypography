#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle {
	public:
		Particle();

		void setup(ofPixels * _pix, ofColor _c, float _globalRotation);
		void update();
		void display();
		void getPosition();
		bool isInText();

		ofPixels * pixPtr;
		ofColor c;
		float globalRotation;

		ofVec2f loc;
		ofVec2f vel;

		float life;
		float lifeRate;
};

#endif // PARTICLE_H
