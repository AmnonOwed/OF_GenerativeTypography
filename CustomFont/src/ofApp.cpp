#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	// use the relative or absolute path to a .ttf file
	// or place the font file in the bin/data folder
	ttf.loadFont("FreeSans.ttf", 155);
	s = "TYPE";
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255);
	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	ofSetColor(0);
	ttf.drawString(s, ofGetWidth() / 2 + offset.x, ofGetHeight() / 2 + offset.y);
}
