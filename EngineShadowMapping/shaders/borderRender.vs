#version 430                                                                        

layout (location = 0) in float vs_type;                                                                                    
layout (location = 1) in vec3 vs_pos;   

out float gs_type;

uniform vec3 cameraPos;                                     

void main()                                                                         
{                           
	gs_type = vs_type;                                                        
    gl_Position = vec4(vs_pos.x, cameraPos.y, vs_pos.z, 1.0);                                              
}  