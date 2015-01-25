#include "Particle.h"

Particle::Particle(){
	pixPtr = NULL;
}

//--------------------------------------------------------------
void Particle::setup(ofPixels * _pix, ofColor _c, int _drawMode){
	pixPtr = _pix;
	c = _c;
	drawMode = _drawMode;

	radius = 10; // base radius of the ellipse
	life = 1; // start with a full life
	lifeRate = 0.01; // decrease rate of life

	getPosition();

	vel.set(1, 0);
	vel.rotate(ofRandom(360));
}

//--------------------------------------------------------------
void Particle::update(){
	vel.rotate(ofRandom(-45, 45)); // rotate velocity (direction of movement)
	loc += vel; // add velocity to position (aka move!)
	switch(drawMode){
	 case 0:
		 // once the particle is outside the text, randomly place it somewhere inside the text
		 if(!isInText()){
			 getPosition();
		 }
		 break;

	 case 1:
	 case 2:
		 life -= lifeRate; // decrease life by the lifeRate (the particle is removed when no life is left)
		 break;

	 case 3:
		 // combine the behaviors of case 0 (keep particle inside text) and 1 (decrease life, remove particle)
		 if(!isInText()){
			 getPosition();
		 }
		 life -= lifeRate;
		 break;
	}
	if(life < 0){
		life = 1;
		getPosition();
	}
}

//--------------------------------------------------------------
void Particle::display(){
	float r = radius; // radius of the ellipse
	switch(drawMode){
	 case 0:
		 break; // don't change radius

	 case 1: // go to 3
	 case 2: // go to 3
	 case 3:
		 r *= life;
		 break; // base the radius of the ellipse on the life (which decreases from 1 to 0)
	}
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
	if(pixPtr){
		return (pixPtr->getColor(loc.x, loc.y) == c);
	}
	else{
		return true;
	}
}
