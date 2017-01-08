/*
 100 Days of GLSL experiments:
 Day 1: mixing colours

 
 */

//#include "helperfuncs.glsl"

uniform vec2 uResolution;
uniform vec2 u_mouse;
uniform float uTime;

#define PI 3.14159265359

out vec4 oColour;

vec3 colorA = vec3(0.92, 0.28,0.00);
vec3 colorB = vec3(0.09,0.64,0.96);

// Plot a line on Y using a value between 0.0-1.0
//float plot(vec2 st, float pct){
//    return  smoothstep( pct-0.02, pct, st.y) -
//    smoothstep( pct, pct+0.02, st.y);
//}

float backInOut(float t) {
    float f = t < 0.5
    ? 2.0 * t
    : 1.0 - (2.0 * t - 1.0);
    
    float g = pow(f, 3.0) - f * sin(f * PI);
    
    return t < 0.5
    ? 0.5 * g
    : 0.5 * (1.0 - g) + 0.5;
}

void main() {
    vec2 st = gl_FragCoord.xy/uResolution;
    
    vec3 color = vec3(0.0);
    
    float pct = backInOut(abs(sin(uTime)));
    
    color = mix(colorA, colorB, pct);
    
    oColour = vec4(color,1.0);
}