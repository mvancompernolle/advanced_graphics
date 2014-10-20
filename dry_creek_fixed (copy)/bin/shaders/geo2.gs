#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 10) out;
in vec3 gsColor[];
in float gsOffset[];
out vec3 fsColor;
uniform vec3 color;
uniform mat4 mvpMatrix;
uniform float terrainScalar;

void main() {
    float offset = gsOffset[0];

    // large grass
    fsColor = color;
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    fsColor = gsColor[0];
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 2.0, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 2.0, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    fsColor = gsColor[0] * 0.75f;
    gl_Position = gl_in[0].gl_Position + vec4(0.0, 2.5, 0.0, 0.0) * terrainScalar;
    EmitVertex();
    EndPrimitive();

    // small crass
    fsColor = color;
    gl_Position = gl_in[0].gl_Position + vec4(-0.4 - offset, 0.0, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.2 - offset, 0.0, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    fsColor = gsColor[0];
    gl_Position = gl_in[0].gl_Position + vec4(-0.4 - offset, 1.3, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.2 - offset, 1.3, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    fsColor = gsColor[0] * 0.75f;
    gl_Position = gl_in[0].gl_Position + vec4(-0.3 - offset, 2.0, 0.0, 0.0) * terrainScalar;
    EmitVertex();

    EndPrimitive();
}