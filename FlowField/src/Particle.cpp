#include "Particle.h"

Particle::Particle(ofPixels & _pix, ofColor & _c, int & _drawMode){
	pix = _pix;
	c = _c;
	drawMode = _drawMode;

	getPosition();

	// set the maximum life of the Particles depending on the drawMode
	switch(drawMode){
	 case 0:
		 maxLife = 1.25;
		 break;

	 case 1:
		 maxLife = 1.0;
		 break;

	 case 2:
		 maxLife = 0.75;
		 break;

	 case 3:
		 maxLife = 0.5;
		 break;
	}

	// randomly set a life and lifeRate for each Particle
	life = ofRandom(0.5 * maxLife, maxLife);
	lifeRate = ofRandom(0.01, 0.02);
}

//--------------------------------------------------------------
void Particle::update(){
	// the velocity/direction of each Particle is based on a flowfield using OpenFrameworks's ofNoise() method
	// drawMode 0: no extras (an xy-position will always return the same angle)
	// drawMode 1: dynamic noise (an xy-position will return a slightly different angle on every frame)
	// drawMode 2: rotation (the angle of each xy-position is globally rotated)
	// drawMode 3: dynamic noise + rotation (combines drawModes 1 & 2)
	float angle = ofNoise(loc.x * 0.01, loc.y * 0.01, drawMode == 1 || drawMode == 3 ? ofGetElapsedTimef() : 0) * 360;
	vel.set(1, 0);
	vel.rotate(angle + (drawMode == 2 || drawMode == 3 ? fmod(ofGetElapsedTimef() * 33, 360) : 0));
	loc += vel;
	life -= lifeRate; // decrease life by the lifeRate (the particle is removed by the addRemoveParticles() method when no life is left)
	if(life < 0){
		getPosition();
		life = ofRandom(0.5 * maxLife, maxLife);
	}
}

//--------------------------------------------------------------
void Particle::display(){
	float r = 8 * life / maxLife; // radius of the ellipse
	ofFill();
	ofSetColor(255);
	ofEllipse(loc.x, loc.y, r, r); // draw ellipse
	ofNoFill();
	ofSetColor(0, 135);
	ofEllipse(loc.x, loc.y, r, r); // draw ellipse
}

//--------------------------------------------------------------
void Particle::getPosition(){
	// get a random position inside the text
	while(!isInText()){
		loc.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	}
}

//--------------------------------------------------------------
bool Particle::isInText(){
	return (pix.getColor(loc.x, loc.y) == c);
}
