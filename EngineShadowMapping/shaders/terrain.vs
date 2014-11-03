#version 430

uniform mat4 mvp;
uniform mat4 model;

in vec3 vs_pos;
in vec2 vs_tex;
in vec3 vs_normal;

out vec2 fs_tex;
out vec3 fs_normal;
out vec3 fs_pos;

void main(void){

	fs_normal = vs_normal;
	fs_tex = vs_tex;
	fs_pos = (model * vec4(vs_pos, 1.0)).xyz;
	gl_Position = mvp * vec4(vs_pos, 1.0);
}
