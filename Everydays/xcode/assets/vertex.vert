#version 150

uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;
uniform float uScale;

in vec4 ciPosition;


void main()
{
    
    gl_Position = ciModelViewProjection * ciPosition;
    
}
