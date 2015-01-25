#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	lineLength = 25; // length of drawn lines
	reverseDrawing = false; // boolean to flip the drawing method (toggle with mouse)
	ellipse_color = ofColor(0); // color of drawn ellipses
	line_color = ofColor(0, 125); // color of drawn lines
	fbo_color = ofColor(0);

	ofBackground(255);
	ofSetLineWidth(0.5);
	ofEnableAntiAliasing();
	ofEnableSmoothing();
	ofSetFrameRate(60);

	ofTrueTypeFont ttf;
	ttf.loadFont(OF_TTF_SANS, 350);
	string s = "TYPE";

	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	pix.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);
	fbo.begin();
	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	ofSetColor(fbo_color);
	ttf.drawString(s, fbo.getWidth() / 2 + offset.x, fbo.getHeight() / 2 + offset.y);
	fbo.end();

	fbo.readToPixels(pix); // the ofPixels class has a convenient getColor() method
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	// determine grid dimensions based on the mouse position and calculate resulting grid settings
	int gridHorizontal = (int)ofMap(mouseX, 0, ofGetWidth(), 30, 200); // number of horizontal grid cells (based on mouseX)
	int gridVertical = (int)ofMap(mouseY, 0, ofGetHeight(), 15, 100); // number of vertical grid cells (based on mouseY)
	float w = (float)ofGetWidth() / gridHorizontal;
	float h = (float)ofGetHeight() / gridVertical;
	float r = MIN(w, h);

	// draw shapes to the screen
	for(int y = 0; y < gridVertical; y++){
		for(int x = 0; x < gridHorizontal; x++){
			float e_x = x * w;
			float e_y = y * h;
			ofColor c = pix.getColor((int)e_x, (int)e_y); // get fbo color at this coordinate
			bool textDrawn = (c == fbo_color); // is the color equal to fbo_color (aka is there text here)
			// use the reverseDrawing boolean to flip the textDrawn boolean
			// thus in fact flipping the resulting displayed shapes
			if(reverseDrawing ? !textDrawn : textDrawn){
				ofFill();
				ofSetColor(ellipse_color);
				ofEllipse(e_x, e_y, r, r);
			}
			else{
				ofNoFill();
				ofSetColor(line_color);
				ofLine(e_x, e_y, e_x + lineLength, e_y + lineLength);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	reverseDrawing = !reverseDrawing; // toggle boolean for drawing method
}
