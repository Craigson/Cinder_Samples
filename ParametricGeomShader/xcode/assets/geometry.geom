#version 150

#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

layout(points) in;
layout(triangle_strip, max_vertices = 64) out;

in vec3 vColor[]; // Output from vertex shader for each vertex
out vec3 gColor; // Output to fragment shader

//uniform int uNumSides;
//uniform float uRadius;
uniform float uAspect;

uniform mat4 ciProjectionMatrix;

const float PI = 3.1415926;

void main()
{
    float size = 10.f;
    // To draw a circle, we'll iterate through 24
    // segments and form triangles
    float twoPi = 6.283185306;
    
    float delta = twoPi / 12.0;
    
    for (float theta = 0.0; theta < twoPi; theta += delta)
    {
        // Draw a triangle to form a wedge of the circle
        gl_Position = gl_in[0].gl_Position + vec4(cos(theta) * size, sin(theta) * size * uAspect, 0.0, 0.0);
        EmitVertex();
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        gl_Position = gl_in[0].gl_Position + vec4(cos(theta + delta) * size, sin(theta + delta) * size * uAspect, 0.0, 0.0);
        EmitVertex();
        EndPrimitive();
    }
}