#define PI 3.14159265359

uniform vec2 uResolution;
uniform float uTime;

out vec4 oColour;

float exponentialInOut(float t) {
    return t == 0.0 || t == 1.0
    ? t
    : t < 0.5
    ? +0.5 * pow(2.0, (20.0 * t) - 10.0)
    : -0.5 * pow(2.0, 10.0 - (t * 20.0)) + 1.0;
}

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}

float box(in vec2 _st, in vec2 _size){
    _size = vec2(0.5) - _size*0.5;
    vec2 uv = smoothstep(_size,
                         _size+vec2(0.001),
                         _st);
    uv *= smoothstep(_size,
                     _size+vec2(0.001),
                     vec2(1.0)-_st);
    return uv.x*uv.y;
}

float cross(in vec2 _st, float _size){
    return  box(_st, vec2(_size,_size/4.)) +
    box(_st, vec2(_size/4.,_size));
}

vec3 iter(vec2 st)
{
    // move space from the center to the vec2(0.0)
    st -= vec2(0.5);
    // rotate the space
    st = rotate2d( exponentialInOut( abs(sin(uTime/4.)) ) * PI ) * st;
    // move it back to the original place
    st += vec2(0.5);
    
    // Show the coordinates of the space on the background
    // color = vec3(st.x,st.y,0.0);
    
    // Add the shape on the foreground
    return vec3( cross(st,0.2) );
}

void main(){
    vec2 st = gl_FragCoord.xy/uResolution.xy;
    vec3 color = vec3(0.0);
    
    // range from -0.5 to 0.5
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++)
        {
            color += iter(st - vec2(-0.5 + float(i)/5., -0.5 + float(j)/5.));
        }
    }
    
    // color += vec3( cross(st,0.25) );
    
    
    
    oColour = vec4(color,1.0);
}