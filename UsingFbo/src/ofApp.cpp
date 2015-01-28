#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);

	ofTrueTypeFont ttf;
	ttf.loadFont(OF_TTF_SANS, 155);
	string s = "TYPE";

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
	fbo.begin();
	ofBackground(255);
	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	ofSetColor(0);
	ttf.drawString(s, fbo.getWidth() / 2 + offset.x, fbo.getHeight() / 2 + offset.y);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255, 0, 0);
	// OpenFrameworks supports color modulation of images and fbo's
	// To display an image / fbo fully opaque, without color modulation,
	// the color must be set to opaque white
	ofSetColor(255);
	fbo.draw(mouseX - ofGetWidth() / 2, mouseY - ofGetHeight() / 2);
}
