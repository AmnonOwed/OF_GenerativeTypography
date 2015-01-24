#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle {
	public:
		Particle(ofPixels & _pix, ofColor & _c, int & _drawMode);

		void update();
		void display();
		void getPosition();
		bool isInText();

		ofPixels pix;
		ofColor c;
		int drawMode;

		ofVec2f loc;
		ofVec2f vel;

		float maxLife;
		float life;
		float lifeRate;
};

#endif // PARTICLE_H
