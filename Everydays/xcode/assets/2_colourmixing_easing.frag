#define PI 3.14159265359

uniform vec2 uResolution;
uniform vec2 u_mouse;
uniform float uTime;

out vec4 oColour;

vec3 colorA = vec3(0.149,0.141,0.912);
vec3 colorB = vec3(1.000,0.833,0.224);

float plot (vec2 st, float pct){
    return  smoothstep( pct-0.01, pct, st.y) -
    smoothstep( pct, pct+0.01, st.y);
}

float cubicPulse( float c, float w, float x ){
    x = abs(x - c);
    if( x>w ) return 0.0;
    x /= w;
    return 1.0 - x*x*(3.0-2.0*x);
}


void main() {
    vec2 st = gl_FragCoord.xy/uResolution.xy;
    vec3 color = vec3(0.0);
    
    vec3 pct = vec3(st.y);
    
    // pct.r = smoothstep(0.1,1.0, st.y);
//    pct.b = abs(sin(uTime)) - cubicPulse(0.5,0.2,st.y);
//    pct.g = pow(st.x,0.9);
    
    float y = step(0.5, st.x+0.3);
    
    color = mix(colorA, colorB, pct);
    
    // pct.z = smoothstep(0.0,1.0,st.y);
    // Plot transition lines for each channel
//     color = mix(color,vec3(1.0,0.0,0.0),plot(st,pct.r));
//     color = mix(color,vec3(0.0,1.0,0.0),plot(st,pct.g));
//     color = mix(color,vec3(0.0,0.0,1.0),plot(st,pct.b));
    
    oColour = vec4(color,1.0);
}