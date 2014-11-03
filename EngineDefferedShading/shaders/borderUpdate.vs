#version 430                                                                        
                                                                                    
layout (location = 0) in float vs_type;                                                
layout (location = 1) in vec3 vs_pos;                                             
layout (location = 2) in vec3 vs_vel;                                             
layout (location = 3) in float vs_age;                                                 
                                                                                    
out float gs_type;                                                                    
out vec3 gs_pos;                                                                 
out vec3 gs_vel;                                                                 
out float gs_age;                                                                     
                                                                                    
void main()                                                                         
{                                                                                   
    gs_type = vs_type;                                                                   
    gs_pos = vs_pos;                                                           
    gs_vel = vs_vel;                                                            
    gs_age = vs_age;                                                                   
}