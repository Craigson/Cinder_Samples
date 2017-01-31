#define TWO_PI 6.28318530718
#define NUM_CIRCLES 12

uniform vec2 uResolution;
uniform float uTime;

out vec4 oColour;

void main(void){

vec2 st = gl_FragCoord.xy/uResolution.xy;
// st.x *= u_resolution.x/u_resolution.y;
// vec3 color = vec3(0.0);
float d = 0.0;

// Remap the space to -1. to 1.
st = st *2.-1.;

// Make the distance field
// d = length( abs(st)- smoothstep(0.,1.,abs(sin(u_time/3.)) ));
d = length( min(abs(st)-.9,0.) * sin(uTime/3.) * 2.*2. );
// d = length( max(abs(st)-.3,0.) );

// Visualize the distance field
    float r = fract( d * abs(cos(uTime/2.)) / 2.);
    float g = fract( d * 6.1 / 2.);
    float b = fract( d/2. * abs( sin(uTime)) * 5.0 );
    
//    r = smoothstep(r - r*0.01, r + r*0.01, r);
//    g = smoothstep(g - g*0.01, g + g*0.01, g);
//    b = smoothstep(b - b*0.01, b + b*0.01, b);
    oColour = vec4(r,g,b,.2);
}