#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	gradientCounter = 0;

	displayStroke = false;
	changeGradientSize = true;

	fg_color = ofColor(255, 0, 0);
	bg_color = ofColor(0, 0, 255);
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
	// move the mouse to change the grid dimensions
	int gridVertical = (int)ofMap(mouseY, 0, ofGetHeight(), 10, 100);
	int gridHorizontal = (int)ofMap(mouseX, 0, ofGetWidth(), 10, 100);
	int margin = MAX(gridVertical, gridHorizontal) * 2;
	// dynamic gradient size (toggle with 'g' key)
	if(changeGradientSize){
		gradientCounter++;
	}
	float gradientSize = 100 + sinf(gradientCounter * 0.01) * 100;
	// create diagonal lines covering the whole screen using two while loops
	// some initial variables needed within the while loops
	int y = 0;
	bool done = false;
	bool insideShape = false;
	while(!done){
		// start somewhere left of the screen, each time a little bit further down
		y += gridVertical;
		int vx = -margin;
		int vy = y;
		mesh = ofMesh();
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP); // ofMesh does not support QUAD_STRIP
		// keep going while the right or top side hasn't been reached
		while(vx <ofGetWidth() + margin && vy> -margin){
			// check if point is inside text
			bool inText = false;
			if(vx >= 0 && vx < pix.getWidth() && vy >= 0 && vy < pix.getHeight()){
				ofColor c = pix.getColor(vx, vy); // get fbo color at this coordinate
				inText = (c == fbo_color);
			}
			if(inText){
				if(!insideShape){
					// end and draw the current mesh when first entering the text
					mesh.draw();
					if(displayStroke){
						mesh.disableColors();
						mesh.drawWireframe();
					}
					insideShape = true;
				}
			}
			else{
				if(insideShape){
					// start a new mesh when exiting the text
					mesh = ofMesh();
					mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
					insideShape = false;
				}
				// when outside text, add two vertices
				mesh.addVertex(ofVec2f(vx, vy));
				mesh.addColor(fg_color);
				mesh.addVertex(ofVec2f(vx + gradientSize, vy + gradientSize));
				mesh.addColor(bg_color);
			}
			// move right and upwards
			vx += gridHorizontal;
			vy -= gridVertical;
			// if we are beyond the right and beyond the bottom of the screen, stop the main while loop
			if(vx > ofGetWidth() && vy > ofGetHeight()){
				done = true; // escape!
			}
		}
		mesh.draw();
		if(displayStroke){
			mesh.disableColors();
			mesh.drawWireframe();
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
	 case 'g':
		 changeGradientSize = !changeGradientSize;
		 break;

	 case 's':
		 displayStroke = !displayStroke;
		 break;
	}
}
