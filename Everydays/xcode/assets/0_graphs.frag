/*
 100 Days of GLSL experiments:
 Day 1: graphing functions
 

 
 */

#define PI 3.14159265359

uniform vec2 uResolution;
uniform vec2 u_mouse;
uniform float uTime;

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
//    float y = 1.0 - pow(abs(st.x), 0.5);
    //    float y = st.x;
    //    float y = step(0.8,st.x);
//    float y = ceil(sin(st.x)) - floor(sin(st.x));
    float y = abs(sin(uTime)) * pow(cos(PI * st.x/2.0), 1.5);
    
    vec3 color = vec3(y);
    
    // Plot a line
    float pct = plot(st,y);
    color = (1.0-pct)*color+pct*vec3(0.0,1.0,0.0);
    
    oColour = vec4(color,1.0);
}