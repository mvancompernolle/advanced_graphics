#version 330

in float colorPos;
uniform sampler1D gSampler;

void main(void){
   gl_FragColor = texture(gSampler, colorPos);
}
