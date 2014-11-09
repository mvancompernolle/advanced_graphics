#version 430

layout (triangles_adjacency) in;
layout (line_strip, max_vertices = 6) out;

in vec3 fs_pos[];

uniform vec3 cameraPos;

void emitLine(int start, int end){

	gl_Position = gl_in[start].gl_Position;
	EmitVertex();

	gl_Position = gl_in[end].gl_Position;
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
	vec3 direction = cameraPos - fs_pos[0];

	// check to see if triangle is facing the camera
	if(dot(normal, direction) > 0.00001){

		// check to see if adjacent triangles are not facing the camera
		normal = cross(e3, e1);

		if(dot(normal, direction) <= 0){
			emitLine(0, 2);
		}

		normal = cross(e4, e5);
		direction = cameraPos - fs_pos[2];

		if(dot(normal, direction) <= 0){
			emitLine(2, 4);
		}

		normal = cross(e2, e6);
		direction = cameraPos - fs_pos[4];

		if(dot(normal, direction) <= 0){
			emitLine(4, 0);
		}
	}
}