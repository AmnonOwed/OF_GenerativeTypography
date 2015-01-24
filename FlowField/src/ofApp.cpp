#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	maxParticles = 250; // the maximum number of active particles
	drawMode = 0; // move through the drawing modes by clicking the mouse

	bg_color = ofColor(255);
	fbo_color = ofColor(0);

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	fbo.begin();
	ofTrueTypeFont ttf;
	ttf.loadFont(OF_TTF_SANS, 350);
	string s = "TYPE";
	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	ofSetColor(fbo_color);
	ttf.drawString(s, fbo.getWidth() / 2 + offset.x, fbo.getHeight() / 2 + offset.y);
	fbo.end();

	fbo.readToPixels(pix); // the ofPixels class has a convenient getColor() method

	ofBackground(bg_color);
	ofSetBackgroundAuto(false);

	ofEnableAntiAliasing();
	ofSetFrameRate(60); // cap frameRate otherwise it goes too fast

	bReset = true;
}

//--------------------------------------------------------------
void ofApp::update(){
	if(bReset){
		resetParticles();
	}
	for(int i = 0; i < particles.size(); i++){
		particles[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// display each particle in the list
	for(int i = 0; i < particles.size(); i++){
		particles[i].display();
	}
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
	bReset = false;
	particles.clear();
	while(particles.size() < maxParticles){
		particles.push_back(Particle(pix, fbo_color, drawMode));
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	drawMode = ++drawMode % 4; // move through 4 drawing modes (0, 1, 2, 3)
	ofBackground(bg_color); // clear the screen when changing drawing mode
	if(drawMode == 2){
		ofSetColor(255);
		fbo.draw(0, 0); // draw text to the screen for drawMode 2
	}
	bReset = true;
}
