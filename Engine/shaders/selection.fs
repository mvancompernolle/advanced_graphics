#version 430

uniform uint gObjectIndex;

out vec3 fragColor;

void main(){

	fragColor = vec3(float(gObjectIndex), 0, 0);
}