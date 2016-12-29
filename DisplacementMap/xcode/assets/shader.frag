#version 150

uniform sampler2D tex0;

in vec2 texCoordVarying;

out vec4 outputColor;
 
void main()
{
    outputColor = texture(tex0, texCoordVarying);
//    outputColor = vec4(1.0);
}