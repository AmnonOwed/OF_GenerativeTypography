#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	saveOneFrame = false; // variable used to save a single frame as a PDF page

	bg_color = ofColor(255);
	fbo_color = ofColor(0);

	ofBackground(bg_color);
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
	ofClear(0, 0, 0, 0);
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
	addCircles(25); // try (not enforced) to add N circles per frame
	for(int i = 0; i < circles.size(); i++){
		circles[i]->update();
		circles[i]->overlap(circles);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(saveOneFrame){
		ofBeginSaveScreenAsPDF("CirclePacking-" + ofGetTimestampString() + ".pdf", false);
	}

	for(int i = 0; i < circles.size(); i++){
		circles[i]->display();
	}

	if(saveOneFrame){
		ofEndSaveScreenAsPDF();
		saveOneFrame = false;
	}
}

//--------------------------------------------------------------
void ofApp::addCircles(int num){
	for(int i = 0; i < num; i++){
		// random point
		int x = (int)ofRandom(ofGetWidth());
		int y = (int)ofRandom(ofGetHeight());
		// is it inside the text?
		ofColor c = pix.getColor(x, y); // get fbo color at this coordinate
		bool inText = (c == fbo_color); // is the color equal to the fbo_color (aka is there text here)
		if(inText){
			// does it overlap with other Circles?
			bool overlap = false;
			for(int i = 0; i < circles.size(); i++){
				if(ofDist(x, y, circles[i]->x, circles[i]->y) <= circles[i]->radius + 2){
					overlap = true;
					break;
				}
			}
			if(!overlap){
				Circle * c = new Circle();
				c->setup(x, y);
				circles.push_back(c);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 's'){
		saveOneFrame = true; // set the variable to true to save a single frame as a PDF file / page
	}
}
