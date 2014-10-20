#version 330

in vec3 v_position;
in vec3 v_normal;
out vec3 out_normal;
out vec3 world_pos;
uniform mat4 mvpMatrix;
uniform mat4 model;
uniform mat4 view;

void main(void){
	gl_Position = mvpMatrix * vec4(v_position, 1.0);
	out_normal = (model * vec4(v_normal, 0.0)).xyz;
	world_pos = (model * vec4(v_position, 1.0)).xyz;
}
