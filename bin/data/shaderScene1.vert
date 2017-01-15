#version 330

uniform mat4 MVP;
uniform mat4 VM;
uniform mat4 TexMat;

layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;
layout(location=2) in vec3 normal;
out vec2 texCoordVarying;
//out vec3 texCoordCube;
out vec3 N;
out vec3 N2;
out vec3 V;
out float R;

float reflectance(vec3 n, vec3 v) {
	float ni=2.0;
    float cosi=dot(n,v);
    float sini=sqrt(1.0-cosi*cosi);
    float sint=1.0/ni*sini;
    float cost=sqrt(1.0-sint*sint);

    //s and p polarized waves
    float rs=(cosi-ni*cost)/(cosi+ni*cost);
    float rp=(cost-ni*cosi)/(cost+ni*cosi);
    float R=(rs*rs+rp*rp)*0.5;
    return min(R,1);
}

void main(){
	texCoordVarying =texcoord;
	gl_Position = MVP*vec4(position,1);
	//the one below is a bit finicky (the eye is at origo even in model coordinates, since there is no translation of camera...)
	//in general does not work
	V=normalize((TexMat*vec4(position,1)-TexMat*vec4(0.0,0.0,0.0,1)).xyz);
	N=normalize((TexMat*vec4(normal,0)).xyz);
	//texCoordCube=2*dirFromEye+normalX;
	R=reflectance(N,V);
	N2=normalize((VM*vec4(normal,0)).xyz);
	
	
}