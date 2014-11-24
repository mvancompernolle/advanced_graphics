#version 430

layout (location = 0) in vec3 vs_pos;

uniform mat4 mvp;

out vec3 fs_tex;

void main(){

    vec4 mvp_pos = mvp * vec4(vs_pos, 1.0);
    gl_Position = mvp_pos.xyww;
    fs_tex = vs_pos;
}