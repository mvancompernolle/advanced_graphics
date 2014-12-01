#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;
in vec3 gs_color[];
in float gs_angle[];
out vec3 fs_color;
uniform mat4 mvp;
uniform vec3 windDir;
uniform vec3 cameraPos;

mat4 rotationMatrix(vec3 axis, float angle){
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main() {

    float dist = distance(vec4(cameraPos, 1.0), gl_in[0].gl_Position);
    //if(dist < 1000.0 && dist > 0){
    mat4 rotMatrix = rotationMatrix(vec3(0, 1, 0), gs_angle[0]);
    vec4 angleVector = rotationMatrix(vec3(0, 1, 0), gs_angle[0]) * vec4(1.0, 0, 0, 0);
    float power = max(0.1, dot(angleVector, vec4(windDir, 0.0)));
    if(dist > 500)
        power = 0;

    // large grass
    fs_color = vec3(0.0, 0.5, 0.0);
    gl_Position = gl_in[0].gl_Position + mvp * rotMatrix * vec4(-0.05, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + mvp * rotMatrix * vec4(0.05, 0.0, 0.0, 0.0);
    EmitVertex();

    fs_color = gs_color[0];
    gl_Position = gl_in[0].gl_Position + mvp * rotMatrix * (vec4(-0.05, 1.0, 0.0, 0.0) + vec4(windDir.x, 0.0, windDir.z, 0.0) * power / 2);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + mvp * rotMatrix * (vec4(0.05, 1.0, 0.0, 0.0) + vec4(windDir.x, 0.0, windDir.z, 0.0) * power / 2);
    EmitVertex();

    fs_color = gs_color[0] * .75;
    gl_Position = gl_in[0].gl_Position + mvp * rotMatrix * (vec4(0.0, 2.5, 0.0, 0.0) + vec4(windDir.x, 0.0, windDir.z, 0.0) * power * 1.5);
    EmitVertex();
    EndPrimitive();
    //}
}