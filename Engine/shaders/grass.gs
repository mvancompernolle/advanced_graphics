#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;
uniform mat4 mvp;

void main() {
    // large grass
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 2.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.1, 2.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, 2.5, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}