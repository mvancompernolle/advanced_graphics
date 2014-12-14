#version 430

in vec2 fs_tex;
uniform sampler2D tex;
uniform vec4 color;

void main(void){
    gl_FragColor = vec4(.5, 0, 0, 1);//texture(tex, fs_tex).a) * color;
}