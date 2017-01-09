#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 PV;


void main()
{
    gl_Position =   PV * vec4(10*position, 1.0);  
    TexCoords = 10*position;
}  