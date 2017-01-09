#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.width=1024;
	settings.height=768;
	settings.setGLVersion(3,3);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());

}
