#version 150

uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;
uniform float uScale;

in vec4 ciPosition;
in vec3 ciNormal;
in vec2	ciTexCoord0;

out vec2 texCoordVarying;
out vec3 vNormal;

out float vScale;

// texture containing incoming noise data
uniform sampler2D tex0;

void main()
{
    
    vNormal = normalize(ciNormalMatrix * ciNormal);

    // we need to scale up the values we get from the texture
    float scale = uScale;
    
    // here we get the red channel value from the texture
    // to use it as displacement along the normal
    float displacementY = texture(tex0, ciTexCoord0).r;
    
    vec3 newPosition = ciPosition.xyz + (ciNormal * displacementY * scale);

    gl_Position = ciModelViewProjection * vec4(newPosition, 1.);
    
    // pass the texture coordinates to the fragment shader
    texCoordVarying = ciTexCoord0;
    
    vScale = clamp(displacementY,0.0,1.0);
    
}
