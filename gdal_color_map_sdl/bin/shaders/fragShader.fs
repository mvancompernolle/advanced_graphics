varying float colorPos;
uniform sampler1D gSampler;

void main(void){
   gl_FragColor = texture1D(gSampler, colorPos);
}
