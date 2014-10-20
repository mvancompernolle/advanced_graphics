#version 330

in vec3 v_position;
uniform mat4 mvpMatrix;

void main(void){
	gl_Position = mvpMatrix * vec4(v_position, 1.0);
}
