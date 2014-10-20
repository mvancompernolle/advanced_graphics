#version 330                                                                        
                                                                                    
layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;                                                       
                                                                                    
uniform mat4 mvp;                                                                   
uniform vec3 cameraPos;                                                            
uniform float size;                                                       
                                                                                    
out vec2 TexCoord;                                                                  
                                                                                    
void main()                                                                         
{                                                                                   
    vec3 pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(cameraPos - pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up) * size;                              
                                                                                    
    pos -= right;                                                                   
    gl_Position = vec4(pos, 1.0);                                             
    TexCoord = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos.y += size;                                                        
    gl_Position = vec4(pos, 1.0);                                             
    TexCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos.y -= size;                                                        
    pos += right;                                                                   
    gl_Position = vec4(pos, 1.0);                                             
    TexCoord = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    pos.y += size;                                                        
    gl_Position = vec4(pos, 1.0);                                             
    TexCoord = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();                                                                 
}                                                                                   
