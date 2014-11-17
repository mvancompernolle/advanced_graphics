#version 430

layout (triangles_adjacency) in;
layout (triangle_strip, max_vertices = 18) out;

in vec3 fs_pos[];

uniform vec3 lightPos;
uniform mat4 vp;

float EPSILON = 30.0;

void emitQuad(int startIndex, vec3 startVertex, int endIndex, vec3 endVertex){

	vec3 lightDir = normalize(startVertex - lightPos);
	vec3 l = lightDir * EPSILON;
	gl_Position = vp * vec4((startVertex + l), 1.0);
	EmitVertex();

	gl_Position = vp * vec4(lightDir, 0.0);
	EmitVertex();

	lightDir = normalize(endVertex - lightPos);
	l = lightDir * EPSILON;
	gl_Position = vp * vec4((endVertex + l), 1.0);
	EmitVertex();

	gl_Position = vp * vec4(lightDir, 0.0);
	EmitVertex();

	EndPrimitive();
}

void main(){

	vec3 e1 = fs_pos[2] - fs_pos[0];
	vec3 e2 = fs_pos[4] - fs_pos[0];
	vec3 e3 = fs_pos[1] - fs_pos[0];
	vec3 e4 = fs_pos[3] - fs_pos[2];
	vec3 e5 = fs_pos[4] - fs_pos[2];
	vec3 e6 = fs_pos[5] - fs_pos[0];

	vec3 normal = cross(e1, e2);
	vec3 direction = lightPos - fs_pos[0];

	// check to see if triangle is facing the camera
	if(dot(normal, direction) > 0.00001){

		// check to see if adjacent triangles are not facing the camera
		normal = cross(e3, e1);

		if(dot(normal, direction) <= 0){
			vec3 startVertex = fs_pos[0];
			vec3 endVertex = fs_pos[2];
			emitQuad(0, startVertex, 2, endVertex);
		}

		normal = cross(e4, e5);
		direction = lightPos - fs_pos[2];

		if(dot(normal, direction) <= 0){
			vec3 startVertex = fs_pos[2];
			vec3 endVertex = fs_pos[4];
			emitQuad(2, startVertex, 4, endVertex);
		}

		normal = cross(e2, e6);
		direction = lightPos - fs_pos[4];

		if(dot(normal, direction) <= 0){
			vec3 startVertex = fs_pos[4];
			vec3 endVertex = fs_pos[0];
			emitQuad(4, startVertex, 0, endVertex);
		}

		vec3 lightDir = (normalize(fs_pos[0] - lightPos)) * EPSILON;
		gl_Position = vp * vec4((fs_pos[0] + lightDir), 1.0);
		EmitVertex();

		lightDir = (normalize(fs_pos[2] - lightPos)) * EPSILON;
		gl_Position = vp * vec4((fs_pos[2] + lightDir), 1.0);
		EmitVertex();

		lightDir = (normalize(fs_pos[4] - lightPos)) * EPSILON;
		gl_Position = vp * vec4((fs_pos[4] + lightDir), 1.0);
		EmitVertex();
		EndPrimitive();
	}
}