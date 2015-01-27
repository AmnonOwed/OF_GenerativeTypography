#include "Particle.h"

Particle::Particle(){
	pixPtr = NULL;
}

//--------------------------------------------------------------
void Particle::setup(ofPixels * _pix, ofColor _c, float _globalRotation){
	pixPtr = _pix;
	c = _c;
	globalRotation = _globalRotation;

	getPosition();

	// randomly set a life and lifeRate for each Particle
	life = ofRandom(0.75, 1.25);
	lifeRate = ofRandom(0.01, 0.02);
}

//--------------------------------------------------------------
void Particle::update(){
	float angle = ofNoise(loc.x * 0.005, loc.y * 0.005) * 360;
	vel.set(1, 0);
	vel.rotate(angle + globalRotation);
	loc += vel;
	life -= lifeRate;
	if(life < 0){
		getPosition();
		life = ofRandom(0.75, 1.25);
		lifeRate = ofRandom(0.01, 0.02);
	}
}

//--------------------------------------------------------------
void Particle::display(){
	bool special = ofRandom(1) < 0.001;
	ofSetLineWidth(special ? ofRandom(0.75, 3) : 0.75);
	ofSetColor(255, special ? ofRandom(175, 255) : 65);
	float pixelSize = special ? 2 : 1;
	ofRect(loc.x, loc.y, pixelSize, pixelSize);
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
