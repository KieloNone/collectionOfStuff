#include "openGLstuff.h"
#include "ofApp.h"
#include "ofMain.h"

GLuint configure_texture(ofImage image){

	//configuring texture
	unsigned char * data=image.getPixels().getData();
	int width = image.getWidth();
	int height = image.getHeight();

    GLuint textureID;
    //creating a texture pointer at address textureID
    glGenTextures(1, &textureID);
    //bind the generated texture to the GL_TEXTURE_2D
    glBindTexture(GL_TEXTURE_2D, textureID);
    //creating the texture (allocating space etc.)
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //setting what to do when the area to draw the texture is either bigger of smaller than the actual texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //linear interpolation when zooming out
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //mipmaps when zooming in
    glGenerateMipmap(GL_TEXTURE_2D); //generating the mipmaps

    return textureID;

}

