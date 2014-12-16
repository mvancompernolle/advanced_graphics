#version 430

layout (location = 0) in vec3 vs_pos;

void main(void){

    gl_Position = vec4(vs_pos, 1.0);
}
