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
		v += FlowField::getVelocity(v);
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
