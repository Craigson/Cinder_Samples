#define PI 3.14159265359
#define TWO_PI 6.28318530718

uniform vec2 uResolution;
uniform vec2 u_mouse;
uniform float uTime;

out vec4 oColour;

float shape(vec2 pos, int numsides, float offset){
    // Number of sides of your shape
    int N = numsides;
    
    // Angle and radius from the current pixel
    float a =  smoothstep(.0,0.8,abs(sin(uTime/2.))) * atan(pos.x,pos.y)+PI;
    float r = TWO_PI/float(N);
    
    // Shaping function that modulate the distance
    float d = cos(floor(.5+a/r)*r-a)*length(pos) + 0.3;
    
    return 1.0-smoothstep(.4,.41,d);
}

// Reference to
// http://thndl.com/square-shaped-shaders.html

void main(){
    vec2 st = gl_FragCoord.xy/uResolution.xy;
    st.x *= uResolution.x/uResolution.y;
    vec3 color = vec3(0.0);
    float d = 0.0;
    
    // Remap the space to -1. to 1.
    st = st *2.-1.;
    
    for (int i = 0; i < 10; i++)
    {
        for ( int j = 0; j < 10; j++)
        {
            color += vec3(shape(st + vec2( 1.088 - float(i)/2.752, 1.000 - float(j)/3.), 3, 0.));
        }
    }
    
    // color = vec3(shape(st + vec2(0.,0.), 3, .0));
    // color += vec3(shape(st + vec2(0.3,0.5) , 3,0.0));
    // color = vec3(d);
    
    oColour = vec4(color,1.0);
}