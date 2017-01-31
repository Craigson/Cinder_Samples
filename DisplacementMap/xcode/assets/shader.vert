#version 150

// these are from the programmable pipeline system
uniform mat4 ciModelViewProjection;

in vec4 ciPosition;
in vec2	ciTexCoord0;

out vec2 texCoordVarying;

// this is how we receive the texture
uniform sampler2D tex0;



void main()
{
    
    // get the position of the vertex relative to the modelViewProjectionMatrix
    vec4 modifiedPosition = ciModelViewProjection * ciPosition;
    
    // we need to scale up the values we get from the texture
    float scale = 1.0f;
    
    // here we get the red channel value from the texture
    // to use it as vertical displacement
    float displacementY = texture(tex0, ciTexCoord0).r;

    // use the displacement we created from the texture data
    // to modify the vertex position
	modifiedPosition.y += displacementY * scale;
    
    // pass the texture coordinates to the fragment shader
    texCoordVarying = ciTexCoord0;
    
    // this is the resulting vertex position
    gl_Position = modifiedPosition;
    

}
