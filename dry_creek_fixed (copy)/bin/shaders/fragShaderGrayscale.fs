#version 330

in vec2 tex;
uniform sampler2D gSampler;

void main(void){
   gl_FragColor = texture(gSampler, tex);
}
