uniform vec2 uResolution;
uniform float uTime;

#define PI 3.14159265358979323846

out vec4 oColour;

uniform bool uReverse;

float exponentialInOut(float t) {
    return t == 0.0 || t == 1.0
    ? t
    : t < 0.5
    ? +0.5 * pow(2.0, (20.0 * t) - 10.0)
    : -0.5 * pow(2.0, 10.0 - (t * 20.0)) + 1.0;
}

vec2 rotate2D(vec2 _st, float _angle){
    _st -= 0.5;
    _st =  mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle)) * _st;
    _st += 0.5;
    return _st;
}

vec2 tile(vec2 _st, float _zoom){
    _st *= _zoom;
    return fract(_st);
}

float box(vec2 _st, vec2 _size, float _smoothEdges){
    _size = vec2(0.5)-_size*0.5;
    vec2 aa = vec2(_smoothEdges*0.5);
    vec2 uv = smoothstep(_size,_size+aa,_st);
    uv *= smoothstep(_size,_size+aa,vec2(1.0)-_st);
    return uv.x*uv.y;
}

void main(void){
    vec2 st = gl_FragCoord.xy/uResolution.xy;
    vec3 color = vec3(0.0);
    
    // Divide the space in 4
    if (uReverse) st = tile(st,5.);
    else st = tile(st + vec2(0.1), 5.);
    
    if (uReverse) {
        
        // Use a matrix to rotate the space 45 degrees
        st = rotate2D( st, PI*0.25 + sin(uTime/2.)*PI    );
//                st = rotate2D( st, PI*0.25);
        
        // Draw a square
        color = 1. - vec3(box(st,vec2(0.7),0.001));
        
    } else {
        // Use a matrix to rotate the space 45 degrees
        st = rotate2D( st, PI*0.25 + sin(uTime/2.)*PI    );
//                st = rotate2D( st, PI*0.25);
        
        // Draw a square
        color = vec3(box(st,vec2(0.71),0.001));
    }
    // color = vec3(st,0.0);
    
    oColour = vec4(color,1.0);
}