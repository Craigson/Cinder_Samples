#define TWO_PI 6.28318530718

uniform vec2 uResolution;
uniform float uTime;

out vec4 oColour;

float diam1 = 1.0;


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
    return  box(_st, vec2(_size,_size/4.));
}

void main(){
    vec2 st = gl_FragCoord.xy/uResolution.xy;
    vec3 color = vec3(0.0);
    
    // larger/outer loop
    color += vec3(box(st,vec2( abs(sin(uTime/2.)))));
    color -= vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.1)));
    
    color += vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.2)));
    color -= vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.3)));
    
    color += vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.4)));
    color -= vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.5)));
    
    color += vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.6)));
    color -= vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.7)));
    
    color += vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.8)));
    color -= vec3(box(st,vec2( abs(sin(uTime/2.)) - 0.9)));
    
    oColour = vec4(color,1.0);
}