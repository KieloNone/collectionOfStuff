#pragma once
#include <vector>
#include "ofMain.h"
//#include "ofxAssimpModelLoader.h"

//ofxAssimpModelLoader model;

ofMatrix4x4 newLookAtMatrixGLMway(const ofVec3f& eye, const ofVec3f& center, const ofVec3f& up);

//very dummy, very finicky implementation. TODO: improve/find a file that is premade
ofMesh readObjFileSimplified(std::string file_path);
