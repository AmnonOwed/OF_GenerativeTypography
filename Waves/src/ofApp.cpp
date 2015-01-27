#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	gridX = 200; // number of horizontal grid points
	gridY = 75; // number of vertical grid points
	waveHeight = 45; // maximum height of each wave (vertex)
	baseHeight = 6; // default base weight of each wave (vertex)

	stroke_color = ofColor(0);

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

}

//--------------------------------------------------------------
void ofApp::draw(){
	float w = float(ofGetWidth()) / gridX;
	float h = float(ofGetHeight()) / gridY;
	ofTranslate(w / 2, h / 2);
	float fc = ofGetElapsedTimef() * 0.35;
	// for each 'row'
	for(int y = 0; y < gridY; y++){
		bool continuous = false;
		// go over all the 'columns'
		for(int x = 0; x < gridX; x++){
			// for each point, determine it's position in the grid
			float vx = x * w;
			float vy = y * h;
			ofColor c = pix.getColor((int)vx, (int)vy); // get fbo color at this coordinate
			bool inText = (c == fbo_color); // is the color equal to the fbo_color (aka is there text here)
			if(inText){
				if(!continuous){
					// when entering the text
					continuous = true;
					path = ofPath();
					fill_color.setHsb(fmod(vx + 2 * vy + ofGetFrameNum(), 255), 191.25, 216.75);
					path.setFillColor(fill_color);
					path.setStrokeColor(stroke_color);
					path.setStrokeWidth(1.5);
					path.moveTo(vx, vy);
				}
				// add a curveVertex point which is moved upwards using noise()
				float n = ofNoise(vx * 0.05 + fc, vy * 0.05, fc);
				vy -= n * n * waveHeight + baseHeight;
				path.curveTo(vx, vy);
			}
			else{
				if(continuous){
					// when exiting the text
					continuous = false;
					path.lineTo(vx, vy);
					path.close();
					path.draw();
				}
			}
		}
	}
}
