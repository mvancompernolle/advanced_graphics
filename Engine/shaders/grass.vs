#version 430

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec3 vs_color;
layout (location = 2) in float vs_angle;
layout (location = 3) in vec3 vs_normal;
out vec3 gs_color;
out vec3 gs_normal;
out float gs_angle;
uniform mat4 mvp;

void main(void){
    gs_normal = vs_normal;
    gs_angle = vs_angle;
    gs_color = vs_color;
	gl_Position = mvp * vec4(vs_position, 1.0);
}
