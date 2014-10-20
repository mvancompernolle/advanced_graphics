#version 330

in vec3 v_position;
in vec2 v_texture;
out vec2 tex;
uniform mat4 mvpMatrix;
uniform float verticalScalar;

void main(void){
	vec3 vert = v_position;
	vert.y = vert.y * verticalScalar;
	gl_Position = mvpMatrix * vec4(vert, 1.0);
	tex = v_texture;
}
