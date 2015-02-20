#ifndef FLOWFIELD_H
#define FLOWFIELD_H

#include "ofMain.h"

class FlowField {
	public:
		static ofVec2f getVelocity(const ofVec2f & pos);
};

#endif // FLOWFIELD_H
