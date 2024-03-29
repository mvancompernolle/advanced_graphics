#version 330

in vec3 v_position;
in float samplePos;
out float colorPos;
out vec3 heightColor;
uniform mat4 mvpMatrix;
uniform float verticalScalar;

void main(void){
	vec3 vert = v_position;
	vert.y = vert.y * verticalScalar;
	gl_Position = mvpMatrix * vec4(vert, 1.0);

	heightColor.xyz = v_position.yyy;
	colorPos = samplePos + .33;
}
