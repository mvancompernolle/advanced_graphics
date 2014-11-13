#version 430                                                                        
                                                                                    
layout (location = 0) in float type;                                                
layout (location = 1) in vec3 pos;                                             
layout (location = 2) in vec3 vel;                                             
layout (location = 3) in float age;                                                 
                                                                                    
out float type0;                                                                    
out vec3 pos0;                                                                 
out vec3 vel0;                                                                 
out float age0;                                                                     
                                                                                    
void main()                                                                         
{                                                                                   
    type0 = type;                                                                   
    pos0 = pos;                                                           
    vel0 = vel;                                                           
    age0 = age;                                                                     
}