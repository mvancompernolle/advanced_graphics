attribute vec3 v_position;
varying float colorPos;
uniform mat4 mvpMatrix;
uniform float verticalScalar;

void main(void){
	vec3 vert = v_position;
	vert.y = vert.y * verticalScalar;
	gl_Position = mvpMatrix * vec4(vert, 1.0);
	colorPos = 0.8;//v_position.y;
}
