#version 430

in vec3 fs_color;

out vec4 color;

void main(void){
	color = vec4(fs_color, 1.0);
}