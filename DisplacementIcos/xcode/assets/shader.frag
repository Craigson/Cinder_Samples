#version 150

uniform sampler2D tex0;

in vec2 texCoordVarying;
in float vScale;

out vec4 outputColor;

void main()
{
//    outputColor = texture(tex0, texCoordVarying);
        outputColor = vec4(1.0) * vec4(vec3(vScale), 1.0);
}