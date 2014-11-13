#version 430                                                                        
                                                                                    
uniform sampler2D colorMap;                                                        
                                                                                    
in vec2 TexCoord;                                                                   
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    FragColor = texture2D(colorMap, TexCoord);                                     
                                                                                    
    if (FragColor.r >= 0.5 && FragColor.g >= 0.5 && FragColor.b >= 0.5) {           
        discard;                                                                    
    }                                                                               
}