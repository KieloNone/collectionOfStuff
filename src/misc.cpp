#include "ofMain.h"



ofMatrix4x4 newLookAtMatrixGLMway(const ofVec3f& eye, const ofVec3f& center, const ofVec3f& up) {
	ofMatrix4x4 m;
	ofVec3f zaxis = (eye - center).getNormalized();
	ofVec3f xaxis = up.getCrossed(zaxis).getNormalized();
	ofVec3f yaxis = zaxis.getCrossed(xaxis);


	m._mat[0].set(xaxis.x, yaxis.x, zaxis.x, 0);
	m._mat[1].set(xaxis.y, yaxis.y, zaxis.y, 0);
	m._mat[2].set(xaxis.z, yaxis.z, zaxis.z, 0);
	m._mat[3].set(-eye.dot(xaxis), -eye.dot(yaxis), -eye.dot(zaxis), 1);
	return m;
}

