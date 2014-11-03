#version 430

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec2 vs_tex;
layout (location = 2) in vec3 vs_normal;

uniform mat4 mvp;

out vec2 fs_tex;

void main(){

	gl_Position = mvp * vec4(vs_pos, 1.0);
	fs_tex = vs_tex;
}