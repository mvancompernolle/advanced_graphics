#version 430

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec2 vs_tex;

out vec2 fs_tex;

void main(void){

	fs_tex = vs_tex;
	gl_Position = vec4(vs_pos, 1.0);
}
