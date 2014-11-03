#version 430                                                                        
                                                                                    
layout(points) in;                                                                  
layout(points) out;                                                                 
layout(max_vertices = 1000) out;                                                      
                                                                                    
in float gs_type[];                                                                   
in vec3 gs_pos[];                                                                
in vec3 gs_vel[];                                                                
in float gs_age[];                                                                    
                                                                                    
out float out_type;                                                                    
out vec3 out_pos;                                                                 
out vec3 out_vel;                                                                 
out float out_age;                                                                     
                                                                                    
uniform float dt;         
uniform float speed;                                                                                              
uniform float generatorTime; 
uniform float xBorderTime;            
uniform float zBorderTime;            

#define TOP_GENERATOR 0.0f                                                                                       
#define TOP_BORDER 1.0f    
#define BOTTOM_GENERATOR 2.0f                                                                                       
#define BOTTOM_BORDER 3.0f 
#define RIGHT_GENERATOR 4.0f                                                                                       
#define RIGHT_BORDER 5.0f
#define LEFT_GENERATOR 6.0f                                                                                       
#define LEFT_BORDER 7.0f                                    
                                                                                                                                                                                                                                       
void main(){

    float age = gs_age[0] + dt;                                        
                 
    // if a top boarder launcher                                                                   
    if (gs_type[0] == TOP_GENERATOR) {                                       
        if (age >= generatorTime) {                                             
            out_type = TOP_BORDER;                                            
            out_pos = gs_pos[0];                                                                                                                          
            out_vel = vec3(speed, 0.0, 0.0);                                      
            out_age = 0.0;                                                             
            EmitVertex();                                                           
            EndPrimitive();                                                         
            age = 0.0;                                                              
        }                                                                           
                                                                                    
        out_type = TOP_GENERATOR;                                             
        out_pos = gs_pos[0];                                                   
        out_vel = gs_vel[0];                                                   
        out_age = age;                                                                 
        EmitVertex();                                                               
        EndPrimitive();                                                             
    } 
    // if a top border particle
    else if(gs_type[0] == TOP_BORDER){
        vec3 DeltaP = dt * gs_vel[0];

        if(age < xBorderTime){
            out_type = TOP_BORDER;                                        
            out_pos = gs_pos[0] + DeltaP;                                  
            out_vel = gs_vel[0];                                  
            out_age = age;                                                         
            EmitVertex();                                                       
            EndPrimitive();  
        }
    }            

    // if a bottom boarder launcher                                                                   
    else if (gs_type[0] == BOTTOM_GENERATOR) {                                       
        if (age >= generatorTime) {                                             
            out_type = BOTTOM_BORDER;                                            
            out_pos = gs_pos[0];                                                                                                                          
            out_vel = vec3(-speed, 0.0, 0.0);                                      
            out_age = 0.0;                                                             
            EmitVertex();                                                           
            EndPrimitive();                                                         
            age = 0.0;                                                              
        }                                                                           
                                                                                    
        out_type = BOTTOM_GENERATOR;                                             
        out_pos = gs_pos[0];                                                   
        out_vel = gs_vel[0];                                                   
        out_age = age;                                                                 
        EmitVertex();                                                               
        EndPrimitive();                                                             
    } 
    // if a bottom border particle
    else if(gs_type[0] == BOTTOM_BORDER){
        vec3 DeltaP = dt * gs_vel[0];

        if(age < xBorderTime){
            out_type = BOTTOM_BORDER;                                        
            out_pos = gs_pos[0] + DeltaP;                                  
            out_vel = gs_vel[0];                                  
            out_age = age;                                                         
            EmitVertex();                                                       
            EndPrimitive();  
        }
    }

    // if a right boarder launcher                                                                   
    else if (gs_type[0] == RIGHT_GENERATOR) {                                       
        if (age >= generatorTime) {                                             
            out_type = RIGHT_BORDER;                                            
            out_pos = gs_pos[0];                                                                                                                          
            out_vel = vec3(0.0, 0.0, -speed);                                      
            out_age = 0.0;                                                             
            EmitVertex();                                                           
            EndPrimitive();                                                         
            age = 0.0;                                                              
        }                                                                           
                                                                                    
        out_type = RIGHT_GENERATOR;                                             
        out_pos = gs_pos[0];                                                   
        out_vel = gs_vel[0];                                                   
        out_age = age;                                                                 
        EmitVertex();                                                               
        EndPrimitive();                                                             
    } 
    // if a right border particle
    else if(gs_type[0] == RIGHT_BORDER){
        vec3 DeltaP = dt * gs_vel[0];

        if(age < zBorderTime){
            out_type = RIGHT_BORDER;                                        
            out_pos = gs_pos[0] + DeltaP;                                  
            out_vel = gs_vel[0];                                  
            out_age = age;                                                         
            EmitVertex();                                                       
            EndPrimitive();  
        }
    }      

    // if a left boarder launcher                                                                   
    else if (gs_type[0] == LEFT_GENERATOR) {                                       
        if (age >= generatorTime) {                                             
            out_type = LEFT_BORDER;                                            
            out_pos = gs_pos[0];                                                                                                                          
            out_vel = vec3(0.0, 0.0, speed);                                      
            out_age = 0.0;                                                             
            EmitVertex();                                                           
            EndPrimitive();                                                         
            age = 0.0;                                                              
        }                                                                           
                                                                                    
        out_type = LEFT_GENERATOR;                                             
        out_pos = gs_pos[0];                                                   
        out_vel = gs_vel[0];                                                   
        out_age = age;                                                                 
        EmitVertex();                                                               
        EndPrimitive();                                                             
    } 
    // if a left border particle
    else if(gs_type[0] == LEFT_BORDER){
        vec3 DeltaP = dt * gs_vel[0];

        if(age < zBorderTime){
            out_type = LEFT_BORDER;                                        
            out_pos = gs_pos[0] + DeltaP;                                  
            out_vel = gs_vel[0];                                  
            out_age = age;                                                         
            EmitVertex();                                                       
            EndPrimitive();  
        }
    }                                                                                                                                                                      
}                                                                                   
