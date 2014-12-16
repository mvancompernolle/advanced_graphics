#version 430

uniform mat4 mvp;

layout (location = 0) in vec3 vs_pos;
layout (location = 1) in vec3 vs_color;
out vec3 gs_pos;
out vec3 gs_color;

void main(void){
    gs_pos = vs_pos;
    gs_color = vs_color;
    gl_Position = mvp * vec4(vs_pos, 1.0);
}