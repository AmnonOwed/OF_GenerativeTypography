#include "MovingLine.h"

MovingLine::MovingLine(){
}

//--------------------------------------------------------------
void MovingLine::setup(vector <ofVec2f> _vertices){
	vertices = _vertices;
}

//--------------------------------------------------------------
void MovingLine::update(){
	for(int i = 0; i < vertices.size(); i++){
		ofVec2f & v = vertices[i];
		v += getVelocity(v);
	}
	vertices[0].x = 0;
}

//--------------------------------------------------------------
void MovingLine::display(float minX, float maxX){
	ofBeginShape();
	ofCurveVertex(minX, vertices[0].y);
	for(int i = 0; i < vertices.size(); i++){
		ofCurveVertex(vertices[i].x, vertices[i].y);
	}
	ofCurveVertex(maxX, vertices.back().y);
	ofEndShape();
}

//--------------------------------------------------------------
// check if the line is completely above the upper boundary (then it will return true)
bool MovingLine::isFullyOutsideBoundaries(float minimum){
	for(int i = 0; i < vertices.size(); i++){
		if(vertices[i].y > minimum){
			return false;
		}
	}
	return true;
}

//--------------------------------------------------------------
// flowfield to generate the velocities for the points in the line
ofVec2f MovingLine::getVelocity(const ofVec2f & pos){
	float factor = 0.005;
	float angle = ofNoise(pos.x * factor, pos.y * factor, ofGetFrameNum() * factor) * 360;
	ofVec2f vel(1, 0);
	vel.rotate(angle);
	vel.limit(0.65); // maximize the movement speed
	return vel;
}
