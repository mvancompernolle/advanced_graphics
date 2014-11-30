#version 430

layout (location = 0) in vec3 v_position;
uniform mat4 mvp;

void main(void){

	gl_Position = mvp * vec4(v_position, 1.0);
}
