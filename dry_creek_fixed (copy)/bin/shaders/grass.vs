#version 330

in vec3 v_position;
in vec3 v_color;
in float v_offset;
out vec3 gsColor;
out float gsOffset;
uniform mat4 mvpMatrix;
uniform float verticalScalar;
uniform vec3 color;

void main(void){
	vec3 vert = v_position;
	vert.y = vert.y * verticalScalar;
	gl_Position = mvpMatrix * vec4(vert, 1.0);
	gsColor = v_color;
	gsOffset = v_offset;
}
