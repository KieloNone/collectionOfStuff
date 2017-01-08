#version 330

uniform mat4 MVP;

layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
out vec2 texCoordVarying;

void main(){
	texCoordVarying =texcoord;
	gl_Position = MVP*vec4(position,1);
}