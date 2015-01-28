#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// set the background color
	bg_color = ofColor(255);

	// cap the frameRate, otherwise it goes too fast
	ofSetFrameRate(60);

	// set the level of multiSampling
	// the maximum is determined by your graphics card
	int multiSampling = 8;
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB, multiSampling);
	fbo.begin();
	ofBackground(bg_color);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
	// draw an ellipse to the fbo
	fbo.begin();
	float x = ofRandom(fbo.getWidth());
	float y = ofRandom(fbo.getHeight());
	ofPushMatrix();
	ofTranslate(x, y);
	float er = ofRandom(5, 45);
	ofColor ec;
	ec.setHsb(ofRandom(255), 255, 255);
	ofFill();
	ofSetColor(ec);
	ofEllipse(0, 0, er, er);
	ofNoFill();
	ofSetColor(0);
	ofEllipse(0, 0, er, er);
	ofPopMatrix();
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// display the fbo (without color modulation)
	ofSetColor(255);
	fbo.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	// clear the fbo
	fbo.begin();
	ofBackground(bg_color);
	fbo.end();
}
