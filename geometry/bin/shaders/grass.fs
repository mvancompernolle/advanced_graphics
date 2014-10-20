#version 330

in vec3 fsColor;
void main(void){
   gl_FragColor = vec4(fsColor, 1.0);
}
