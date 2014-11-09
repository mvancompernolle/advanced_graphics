#version 430

layout (location = 0) in vec3 vs_pos;

uniform mat4 mvp;

void main(){

	gl_Position = mvp * vec4(vs_pos, 1.0);
}