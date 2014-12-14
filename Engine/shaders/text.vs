#version 430

layout (location = 0) in vec4 vs_coord;
out vec2 fs_tex;

void main(void){
    gl_Position = vec4(vs_coord.xy, 0, 1);
    fs_tex = vs_coord.zw;
}