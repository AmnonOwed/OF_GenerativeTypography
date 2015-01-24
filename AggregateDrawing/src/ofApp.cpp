#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	drawSpeed = 200; // number of drawn shapes per draw() call
	drawMode = 0; // move through the drawing modes by clicking the mouse

	BACKGROUND_COLOR = ofColor(255);
	FBO_COLOR = ofColor(0);

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	fbo.begin();
	ofTrueTypeFont ttf;
	ttf.loadFont(OF_TTF_SANS, 350);
	string s = "TYPE";
	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	ofSetColor(FBO_COLOR);
	ttf.drawString(s, fbo.getWidth() / 2 + offset.x, fbo.getHeight() / 2 + offset.y);
	fbo.end();

	fbo.readToPixels(pix); // the ofPixels class has a convenient getColor() method

	ofBackground(BACKGROUND_COLOR);
	ofSetBackgroundAuto(false);

	ofSetRectMode(OF_RECTMODE_CENTER);

	ofEnableAntiAliasing();
	ofSetFrameRate(60); // cap frameRate otherwise it goes too fast
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	// This for loop ensures the code is repeated 'drawSpeed' times
	for(int i = 0; i < drawSpeed; i++){
		// pick a random coordinate
		float x = ofRandom(ofGetWidth());
		float y = ofRandom(ofGetHeight());
		// check if the coordinate is inside the text (in the offscreen fbo's pixels)
		bool insideText = (pix.getColor(x, y) == FBO_COLOR);
		// if it is indeed, then draw a shape in the main screen
		if(insideText){
			// switch based on the current draw mode (move through them by clicking the mouse)
			// each drawing mode has custom settings (stroke, fill, shape, rotation)
			// note that the ofColor HSB range is from 0 to 255
			// note that ofRotate works in degrees
			ofPushMatrix();
			ofTranslate(x, y);
			switch(drawMode){
			 case 0: {
				 float er = ofRandom(5, 45);
				 ofColor ec;
				 ec.setHsb(ofRandom(255), 255, 255);
				 ofFill();
				 ofSetColor(ec);
				 ofEllipse(0, 0, er, er);
				 ofNoFill();
				 ofSetColor(0);
				 ofEllipse(0, 0, er, er);
				 break;
			 }

			 case 1: {
				 float td = ofRandom(3, 10);
				 float tr = ofRandom(360);
				 ofColor tc;
				 tc.setHsb(ofRandom(127.5, 212.5), 255, ofRandom(127.5, 255));
				 ofFill();
				 ofSetColor(tc);
				 ofRotate(tr);
				 ofTriangle(0, -td, -td, td, td, td);
				 break;
			 }

			 case 2: {
				 float rw = ofRandom(5, 20);
				 float rh = ofRandom(5, 50);
				 float rr = ofRandom(360);
				 ofColor rc;
				 rc.setHsb(ofRandom(15), ofRandom(178.5, 255), ofRandom(51, 255));
				 ofRotate(rr);
				 ofFill();
				 ofSetColor(rc);
				 ofRect(0, 0, rw, rh);
				 ofNoFill();
				 ofSetColor(0);
				 ofRect(0, 0, rw, rh);
				 break;
			 }
			}
			ofPopMatrix();
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ofBackground(BACKGROUND_COLOR); // clear the screen when changing drawing mode
	drawMode = ++drawMode % 3; // move through 3 drawing modes (0, 1, 2)
}
