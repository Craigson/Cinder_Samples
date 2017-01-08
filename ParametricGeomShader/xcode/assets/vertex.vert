#version 150

in vec4 ciPosition;
in vec3 ciColor;

uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;

out vec3 vColor;

void main()
{
    gl_Position = ciModelViewProjection * ciPosition;
    vColor = ciColor;
}