#version 430                                                                        
                                                                                    
layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 12) out;  

in float gs_type[];                                                     
                                                                                    
uniform mat4 mvp;                                                                   
uniform vec3 cameraPos;                                                            
uniform float width;
uniform float height;    
                                                                                     
#define TOP_GENERATOR 0.0f                                                                                       
#define TOP_BORDER 1.0f    
#define BOTTOM_GENERATOR 2.0f                                                                                       
#define BOTTOM_BORDER 3.0f 
#define RIGHT_GENERATOR 4.0f                                                                                       
#define RIGHT_BORDER 5.0f
#define LEFT_GENERATOR 6.0f                                                                                       
#define LEFT_BORDER 7.0f                                                    
                                                                                    
void main()                                                                         
{                                                                                   
    vec3 pos = gl_in[0].gl_Position.xyz;                                                                  
                                  
    if(gs_type[0] == TOP_BORDER || gs_type[0] == BOTTOM_BORDER || gs_type[0] == TOP_GENERATOR || gs_type[0] == BOTTOM_GENERATOR){

        // generate center rectangle
        // top left
        gl_Position = mvp * vec4(pos.x - width/2, cameraPos.y + height/2, pos.z, 1.0);
        EmitVertex();   
        // bottom left
        gl_Position = mvp * vec4(pos.x - width/2, cameraPos.y - height/2, pos.z, 1.0);
        EmitVertex();        
        // top right
        gl_Position = mvp * vec4(pos.x + width/2, cameraPos.y + height/2, pos.z, 1.0);
        EmitVertex();        
        // bottom right
        gl_Position = mvp * vec4(pos.x + width/2, cameraPos.y - height/2, pos.z, 1.0);
        EmitVertex();      
        EndPrimitive();      

        // generate top rectangle
        // top left
        gl_Position = mvp * vec4(pos.x - width/2, cameraPos.y + height/2 + 30, pos.z, 1.0);
        EmitVertex();   
        // bottom left
        gl_Position = mvp * vec4(pos.x - width/2, cameraPos.y - height/2 + 30, pos.z, 1.0);
        EmitVertex();        
        // top right
        gl_Position = mvp * vec4(pos.x + width/2, cameraPos.y + height/2 + 30, pos.z, 1.0);
        EmitVertex();        
        // bottom right
        gl_Position = mvp * vec4(pos.x + width/2, cameraPos.y - height/2 + 30, pos.z, 1.0);
        EmitVertex();      
        EndPrimitive(); 

        // generate bottom rectangle
        // top left
        gl_Position = mvp * vec4(pos.x - width/2, cameraPos.y + height/2 - 30, pos.z, 1.0);
        EmitVertex();   
        // bottom left
        gl_Position = mvp * vec4(pos.x - width/2, cameraPos.y - height/2 - 30, pos.z, 1.0);
        EmitVertex();        
        // top right
        gl_Position = mvp * vec4(pos.x + width/2, cameraPos.y + height/2 - 30, pos.z, 1.0);
        EmitVertex();        
        // bottom right
        gl_Position = mvp * vec4(pos.x + width/2, cameraPos.y - height/2 - 30, pos.z, 1.0);
        EmitVertex();      
        EndPrimitive();
    }      
    else if(gs_type[0] == RIGHT_BORDER || gs_type[0] == LEFT_BORDER || gs_type[0] == RIGHT_GENERATOR || gs_type[0] == LEFT_GENERATOR){

        // generate center rectangle
        // top left
        gl_Position = mvp * vec4(pos.x, cameraPos.y + height/2, pos.z - width/2, 1.0);
        EmitVertex();   
        // bottom left
        gl_Position = mvp * vec4(pos.x, cameraPos.y - height/2, pos.z - width/2, 1.0);
        EmitVertex();        
        // top right
        gl_Position = mvp * vec4(pos.x, cameraPos.y + height/2, pos.z + width/2, 1.0);
        EmitVertex();        
        // bottom right
        gl_Position = mvp * vec4(pos.x, cameraPos.y - height/2, pos.z + width/2, 1.0);
        EmitVertex();      
        EndPrimitive();  

        // generate center rectangle
        // top left
        gl_Position = mvp * vec4(pos.x, cameraPos.y + height/2 + 30, pos.z - width/2, 1.0);
        EmitVertex();   
        // bottom left
        gl_Position = mvp * vec4(pos.x, cameraPos.y - height/2 + 30, pos.z - width/2, 1.0);
        EmitVertex();        
        // top right
        gl_Position = mvp * vec4(pos.x, cameraPos.y + height/2 + 30, pos.z + width/2, 1.0);
        EmitVertex();        
        // bottom right
        gl_Position = mvp * vec4(pos.x, cameraPos.y - height/2 + 30, pos.z + width/2, 1.0);
        EmitVertex();      
        EndPrimitive();   

        // generate center rectangle
        // top left
        gl_Position = mvp * vec4(pos.x, cameraPos.y + height/2 - 30, pos.z - width/2, 1.0);
        EmitVertex();   
        // bottom left
        gl_Position = mvp * vec4(pos.x, cameraPos.y - height/2 - 30, pos.z - width/2, 1.0);
        EmitVertex();        
        // top right
        gl_Position = mvp * vec4(pos.x, cameraPos.y + height/2 - 30, pos.z + width/2, 1.0);
        EmitVertex();        
        // bottom right
        gl_Position = mvp * vec4(pos.x, cameraPos.y - height/2 - 30, pos.z + width/2, 1.0);
        EmitVertex();      
        EndPrimitive();       
    }                                                                                                         
                                                                                                                                        
}                                                                                   
