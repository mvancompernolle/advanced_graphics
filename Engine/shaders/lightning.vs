#version 430

uniform mat4 mvp;

layout (location = 0) in vec3 vs_pos;
out vec3 gs_pos;

void main(void){
    gs_pos = vs_pos;
    gl_Position = mvp * vec4(vs_pos, 1.0);
}