#version 430

uniform mat4 mvp;

in vec3 vs_pos;
in vec3 vs_color;

out vec3 fs_color;

void main(void){

	fs_color = vs_color;
	gl_Position = mvp * vec4(vs_pos, 1.0);
}
