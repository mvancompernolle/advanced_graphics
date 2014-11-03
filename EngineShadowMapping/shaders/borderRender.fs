#version 430                                                                        
                                                                                                                                                   
out vec4 FragColor;    

uniform float mixVal;                                                             
                                                                                    
void main(){

    FragColor = mix(vec4(0.0, 0.0, 1.0, 0.1), vec4(1.0, 0.0, 0.0, 1.0), mixVal);                                     
}