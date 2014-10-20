#version 330                                                                        
                                                                                    
layout(points) in;                                                                  
layout(points) out;                                                                 
layout(max_vertices = 500) out;                                                      
                                                                                    
in float type0[];                                                                   
in vec3 pos0[];                                                                
in vec3 vel0[];                                                                
in float age0[];                                                                    
                                                                                    
out float type1;                                                                    
out vec3 pos1;                                                                 
out vec3 vel1;                                                                 
out float age1;                                                                     
                                                                                    
uniform float dt;                                                     
uniform float time;                                                                
uniform sampler1D randomTexture;                                                   
uniform float launcherLifeTime;                                                    
uniform float shellLifeTime;                                                       
uniform float secShellLifeTime;                                              
                                                                                    
#define PARTICLE_TYPE_LAUNCHER 0.0f                                                 
#define PARTICLE_TYPE_SHELL 1.0f                                                    
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f                                          
                                                                                    
vec3 GetRandomDir(float TexCoord)                                                   
{                                                                                   
     vec3 Dir = texture(randomTexture, TexCoord).xyz;                              
     Dir -= vec3(0.5, 0.5, 0.5);                                                    
     return Dir;                                                                    
}                                                                                   
                                                                                    
void main()                                                                         
{                                                                                   
    float age = age0[0] + dt;                                         
                                                                                    
    if (type0[0] == PARTICLE_TYPE_LAUNCHER) {                                       
        if (age >= launcherLifeTime) {                                             
            type1 = PARTICLE_TYPE_SHELL;                                            
            pos1 = pos0[0];                                               
            vec3 Dir = GetRandomDir(time/1000.0);                                  
            Dir.y = max(Dir.y, 0.5);                                                
            vel1 = normalize(Dir) * 10;                                      
            age1 = 0.0;                                                             
            EmitVertex();                                                           
            EndPrimitive();                                                         
            age = 0.0;                                                              
        }                                                                           
                                                                                    
        type1 = PARTICLE_TYPE_LAUNCHER;                                             
        pos1 = pos0[0];                                                   
        vel1 = vel0[0];                                                   
        age1 = age;                                                                 
        EmitVertex();                                                               
        EndPrimitive();                                                             
    }                                                                               
    else {                                                                          
        float DeltaTimeSecs = dt / 1000.0f;                           
        float t1 = age0[0] / 1000.0;                                                
        float t2 = age / 1000.0;                                                    
        vec3 DeltaP = DeltaTimeSecs * vel0[0];                                 
        vec3 DeltaV = DeltaTimeSecs * vec3(0.0, -.098, 0.0);                      
                                                                                    
        if (type0[0] == PARTICLE_TYPE_SHELL)  {                                     
	        if (age < shellLifeTime) {                                             
	            type1 = PARTICLE_TYPE_SHELL;                                        
	            pos1 = pos0[0] + DeltaP;                                  
	            vel1 = vel0[0] + DeltaV;                                  
	            age1 = age;                                                         
	            EmitVertex();                                                       
	            EndPrimitive();                                                     
	        }                                                                       
            else {                                                                  
                for (int i = 0 ; i < 75 ; i++) {                                    
                     type1 = PARTICLE_TYPE_SECONDARY_SHELL;                         
                     pos1 = pos0[0];                                      
                     vec3 Dir = GetRandomDir((time + i)/1000.0);                   
                     vel1 = normalize(Dir) * 10;                             
                     age1 = 0.0f;                                                   
                     EmitVertex();                                                  
                     EndPrimitive();                                                
                }                                                                   
            }                                                                       
        }                                                                           
        else {                                                                      
            if (age < secShellLifeTime) {                                    
                type1 = PARTICLE_TYPE_SECONDARY_SHELL;                              
                pos1 = pos0[0] + DeltaP;                                  
                vel1 = vel0[0] + DeltaV;                                  
                age1 = age;                                                         
                EmitVertex();                                                       
                EndPrimitive();                                                     
            }                                                                       
        }                                                                           
    }                                                                               
}                                                                                   
