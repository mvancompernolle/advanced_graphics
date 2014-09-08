attribute vec3 v_position;
attribute vec3 v_normal;

varying vec3 out_normal;
varying vec3 worldPos;
varying float colorPos;

uniform float verticalScalar;
uniform mat4 mvpMatrix;
uniform mat4 world;

void main(){
	// change the height based on the color of the texture
	vec3 vert = v_position;
	vert.y = vert.y * verticalScalar;
	gl_Position = mvpMatrix * vec4(vert, 1.0);

	// calculate color on color map
	colorPos = v_position.y + .36;

	out_normal = (world * vec4(v_normal, 0.0)).xyz;
	worldPos = (world * vec4(v_position, 1.0)).xyz;
}
