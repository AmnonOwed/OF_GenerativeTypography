#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAntiAliasing();
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofEnableDepthTest();

	bResample = true;
	resampleSpacing = 5.0;
	letterThickness = 145.0;

	ofTrueTypeFont ttf;
	// to extract points, loadFont must be called with makeContours set to true
	ttf.loadFont(OF_TTF_SANS, 300, true, false, true);
	string s = "TYPE";

	vector <ofTTFCharacter> letters = ttf.getStringAsPoints(s);

	for(int i = 0; i < letters.size(); i++){
		ofPath resampledPath = bResample ? resamplePath(letters[i], resampleSpacing) : letters[i];
		ofMesh meshLetter = createMeshFromPath(resampledPath, letterThickness);
		mesh.append(meshLetter);
	}

	getBoundaries(mesh);

	directionalLight.setDiffuseColor(ofColor(125));
	directionalLight.setPosition(boundsMax);
	directionalLight.enable();

	cam.setVFlip(true);

	bReset = true;
}

//--------------------------------------------------------------
void ofApp::update(){
	if(bReset){
		int numPoints = ofRandom(500, 1000);
		float voronoiWallThickness = ofRandom(0.35, 1);
		float startColor = ofRandom(100, 140);

		string title;
		title += "numPoints: " + ofToString(numPoints);
		title += " | voronoiWallThickness: " + ofToString(voronoiWallThickness);
		title += " | startColor: " + ofToString(startColor);
		ofSetWindowTitle(title);

		generatePoints(numPoints);
		createVoronoi(voronoiWallThickness);
		generateColors(startColor);

		calculateCenter();
		cam.setDistance(1.25 * ABS(center.x));

		bReset = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(colors[0], colors[1], OF_GRADIENT_CIRCULAR);

	cam.begin();
	ofPushMatrix();

	ofTranslate(center.x, center.y, center.z);

	ofEnableLighting();

	for(int i = 0; i < cellMeshes.size(); i++){
		ofSetColor(colors[i]);
		cellMeshes[i].draw();
	}

	ofSetColor(0, 75);
	for(int i = 0; i < cellMeshWireframes.size(); i++){
		cellMeshWireframes[i].draw();
	}

	ofDisableLighting();

	ofPopMatrix();
	cam.end();

	ofSetColor(0);
	ofDrawBitmapString("Press space to generate new shape", 10, 20);
}

//--------------------------------------------------------------
ofPath ofApp::resamplePath(ofPath path, float spacing){
	ofPath resampledPath;
	vector <ofPolyline> polylines = path.getOutline();
	for(int j = 0; j < polylines.size(); j++){
		ofPolyline spacePoly = polylines[j].getResampledBySpacing(spacing);
		resampledPath.moveTo(spacePoly[0]);
		for(int k = 1; k < spacePoly.size(); k++){
			resampledPath.lineTo(spacePoly[k]);
		}
		resampledPath.lineTo(spacePoly[0]);
	}
	return resampledPath;
}

//--------------------------------------------------------------
ofMesh ofApp::createMeshFromPath(ofPath path, float thickness){
	ofMesh mesh;
	// get outline of original path (for sides)
	vector <ofPolyline> outline = path.getOutline();
	// add front to mesh
	mesh.append(path.getTessellation());
	// get number of vertices in original path
	int numVerts = mesh.getNumVertices();
	// define offset based on thickness
	ofVec3f offset = ofVec3f(0, 0, -thickness);
	// translate path
	path.translate(offset);
	// add back to mesh
	mesh.append(path.getTessellation());
	// add sides to mesh (via indices)
	for(int j = 0; j < outline.size(); j++){
		int outlineSize = outline[j].getVertices().size();
		vector <int> outlineIndices;
		for(int i = 1; i < outlineSize; i++){
			ofVec3f & v = outline[j].getVertices()[i];
			int originalIndex = -1;
			for(int k = 0; k < numVerts; k++){
				if(v.match(mesh.getVertices()[k])){
					originalIndex = k;
					break;
				}
			}
			outlineIndices.push_back(originalIndex);
		}
		for(int i = 0; i < outlineIndices.size(); i++){
			const int a = outlineIndices[i];
			const int b = outlineIndices[(i + 1) % outlineIndices.size()];
			const int c = outlineIndices[i] + numVerts;
			const int d = outlineIndices[(i + 1) % outlineIndices.size()] + numVerts;
			mesh.addTriangle(a, b, c);
			mesh.addTriangle(c, b, d);
		}
	}
	return mesh;
}

//--------------------------------------------------------------
void ofApp::getBoundaries(ofMesh & mesh){
	vector <ofVec3f> & vertices = mesh.getVertices();
	for(int i = 0; i < vertices.size(); i++){
		ofVec3f & v = vertices[i];
		if(v.x < boundsMin.x){
			boundsMin.x = v.x;
		}
		if(v.y < boundsMin.y){
			boundsMin.y = v.y;
		}
		if(v.z < boundsMin.z){
			boundsMin.z = v.z;
		}
		if(v.x > boundsMax.x){
			boundsMax.x = v.x;
		}
		if(v.y > boundsMax.y){
			boundsMax.y = v.y;
		}
		if(v.z > boundsMax.z){
			boundsMax.z = v.z;
		}
	}
}

//--------------------------------------------------------------
void ofApp::generatePoints(int numPoints){
	points.clear();
	while(points.size() < numPoints){
		ofPoint p = getPointInMesh();
		points.push_back(p);
	}
}

//--------------------------------------------------------------
ofVec3f ofApp::getPointInMesh(){
	ofVec3f p = getRandomPointWithinBounds();
	while(!ofxPointInMesh::isInside(p, mesh)){
		p = getRandomPointWithinBounds();
	}
	return p;
}

//--------------------------------------------------------------
ofVec3f ofApp::getRandomPointWithinBounds(){
	float x = ofRandom(boundsMin.x, boundsMax.x);
	float y = ofRandom(boundsMin.y, boundsMax.y);
	float z = ofRandom(boundsMin.z, boundsMax.z);
	return ofVec3f(x, y, z);
}

//--------------------------------------------------------------
void ofApp::createVoronoi(float wallThickness){
	cellMeshes.clear();
	cellMeshWireframes.clear();

	voro::container con(boundsMin.x, boundsMax.x, // min/max X
						boundsMin.y, boundsMax.y, // min/max Y
						boundsMin.z, boundsMax.z, // min/max Z
						1, 1, 1,
						true, true, true,         // can cells go beyond X/Y/Z boundaries
						8);

	for(int i = 0; i < points.size(); i++){
		addCellSeed(con, points[i], i, false);
	}

	cellMeshes = getCellsFromContainer(con, wallThickness);
	cellMeshWireframes = getCellsFromContainer(con, wallThickness, true);
}

//--------------------------------------------------------------
void ofApp::generateColors(float start){
	float spread = 40;
	colors.clear();
	for(int i = 0; i < cellMeshes.size(); i++){
		ofColor c;
		c.setHsb(fmod(ofRandom(start, start + spread), 255), 255, 255);
		colors.push_back(c);
	}
}

//--------------------------------------------------------------
void ofApp::calculateCenter(){
	for(int i = 0; i < cellMeshes.size(); i++){
		center += cellMeshes[i].getCentroid();
	}
	center /= cellMeshes.size();
	center *= -1;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' '){
		bReset = true;
	}
}
