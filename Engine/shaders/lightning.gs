#version 430

layout(points) in;
layout(line_strip, max_vertices = 246) out;

uniform mat4 mvp;
uniform vec3 positions[10];
uniform sampler1D randomTexture;
uniform float time;

in vec3 gs_pos[];
in vec3 gs_color[];

out vec3 fs_color;

void main(void){
    float maxDist = 250;
    fs_color = gs_color[0];

    // loop through each enemy position and create line if close
    for(int i=0; i<10; i++){

        if(distance(positions[i], gs_pos[0]) < maxDist){
            vec3 dir = positions[i] - gs_pos[0];

            gl_Position = mvp * vec4(gs_pos[0], 1.0);
            EmitVertex();

            for(float j=1.0; j<10; j++){
                gl_Position = mvp * vec4(gs_pos[0] + (dir * j/10.0) 
                + (vec3(texture(randomTexture, (time/10+j*5)).xyz - vec3(.5,.5,.5)) * maxDist/25).xyz,
                1.0);
                EmitVertex();
            }

            /*gl_Position = mvp * vec4(gs_pos[0] + (dir * .2) 
            + (vec3(texture(randomTexture, (time/1000+1)).xyz - vec3(.5,.5,.5)) * maxDist/50).xyz,
            1.0);
            EmitVertex();

            gl_Position = mvp * vec4(gs_pos[0] + (dir * .4) 
            + (vec3(texture(randomTexture, (time/1000+2)).xyz - vec3(.5,.5,.5)) * maxDist/50).xyz,
            1.0);
            EmitVertex();

            gl_Position = mvp * vec4(gs_pos[0] + (dir * .6) 
            + (vec3(texture(randomTexture, (time/1000+3)).xyz - vec3(.5,.5,.5)) * maxDist/50).xyz,
            1.0);
            EmitVertex();

            gl_Position = mvp * vec4(gs_pos[0] + (dir * .8) 
            + (vec3(texture(randomTexture, (time/1000+4)).xyz - vec3(.5,.5,.5)) * maxDist/50).xyz,
            1.0);
            EmitVertex();*/

            gl_Position = mvp * vec4(positions[i], 1.0);
            EmitVertex();
            EndPrimitive();
        }
    }
}