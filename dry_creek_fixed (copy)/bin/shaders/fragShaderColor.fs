#version 330

in vec3 colorPos;
uniform sampler1D gSampler;

void main(void){
   gl_FragColor = vec4(colorPos, 1.0);
}
