#include "FlowField.h"

ofVec2f FlowField::getVelocity(const ofVec2f & pos){
	float factor = 0.005;
	float angle = ofNoise(pos.x * factor, pos.y * factor, ofGetFrameNum() * factor) * 360;
	ofVec2f vel(0.5, 0);
	vel.rotate(angle);
	return vel;
}
