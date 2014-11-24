#version 430                                                                        
                                                                                    
layout(points) in;                                                                  
layout(points) out;                                                                 
layout(max_vertices = 1) out;                                                      
                                                                                    
in float type0[];                                                                   
in vec3 pos0[];                                                                
in vec3 vel0[];                                                                
in float age0[];                                                                    
                                                                                    
out float type1;                                                                    
out vec3 pos1;                                                                 
out vec3 vel1;                                                                 
out float age1;    

uniform vec3 center;  
uniform float dt;                                                                                                                                                                                                                                                                         
                                                                                    
void main(){
    
    // emit vertex                                                                           
    vec3 deltaP = dt/1000 * vel0[0];                                 
    vec3 deltaV = dt/1000 * (center - pos0[0]) * 1.0; 
    type1 = 0;                                        
    pos1 = pos0[0] + deltaP;                                  
    vel1 = vel0[0] + deltaV;                                  
    age1 = age0[0];                                                         
    EmitVertex();                                                       
    EndPrimitive();  
}                                                                                   
