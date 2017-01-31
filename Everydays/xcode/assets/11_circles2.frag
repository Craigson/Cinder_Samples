#define TWO_PI 6.28318530718
#define NUM_CIRCLES 12

uniform vec2 uResolution;
uniform float uTime;

out vec4 oColour;

// circle with antialiased edges
float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
    return smoothstep(_radius-(_radius*0.01),
                      _radius+(_radius*0.01),
                      dot(dist,dist)*4.0);
}

struct Ball {
    vec2 velocity;
    vec2 position;
} myBall;


void main(){
    vec2 st = gl_FragCoord.xy/uResolution.xy;
    vec4 color = vec4(vec3(0.05), 1.0);
    
    float rad = 0.07;
    
    for ( int i = 0; i < NUM_CIRCLES; i++)
    {
        float offset = float(i);
        st.x += rad * cos(uTime/12. * offset);
        st.y += rad * sin(uTime/12. * offset);
        
        float circleRad = 0.02 * float(i);
        
        if ( i % 2 == 0) color -= vec4( vec3(circle(st,circleRad)), 1.);
        else color += vec4(vec3(circle(st, circleRad)), 1.);
    }
    
    
    oColour = color;
}