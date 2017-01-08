/*
 100 Days of GLSL experiments:
 Day 0: step() and smoothstep()
 
 Exploring GLSL's hardware accelerated interpolation methods.  step() interpolation takes two params, the first of which is the threshold, the second is the val to interpolate. Anything under the threshold will return as 0.0, everything above will be 1.0;
 
 smoothstep() interpolates over a range (first two params), whereas the third param is the value to be interpolated.
 
 */


uniform vec2 uResolution;
uniform vec2 u_mouse;
uniform float uTime;

#define PI 3.14159265359

out vec4 oColour;

// Plot a line on Y using a value between 0.0-1.0
float plot(vec2 st, float pct){
    return  smoothstep( pct-0.02, pct, st.y) -
    smoothstep( pct, pct+0.02, st.y);
}

void main() {
    vec2 st = gl_FragCoord.xy/uResolution;
    
//    float powVal = abs(sin(u_time)/5.0) * 20.0;
    
//    float y = pow(st.x,powVal);
//    float y = st.x; // y=x is the equation for a straight line
//    float y = exp(st.x);
//    float y = pow(
    float y = smoothstep(abs(sin(uTime)) * 0.5,0.5,st.x) - smoothstep(0.5,0.5+abs(sin(uTime)) * 0.5,st.x);
//    float y = st.x;
//    float y = step(0.8,st.x);
    
    vec3 color = vec3(y);
    
    // Plot a line
    float pct = plot(st,y);
    color = (1.0-pct)*color+pct*vec3(0.0,1.0,0.0);
    
    oColour = vec4(color,1.0);
}