#version 430

uniform mat4 mvp;
uniform mat4 model;

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec2 vs_tex;
layout (location = 2) in vec3 vs_normal;

out vec2 fs_tex;
out vec3 fs_normal;
out vec3 fs_pos;

void main(void){

	fs_normal = (model * vec4(vs_normal, 0.0)).xyz;
	fs_tex = vs_tex;
	fs_pos = (model * vec4(vs_pos, 1.0)).xyz;
	gl_Position = mvp * vec4(vs_pos, 1.0);
}
