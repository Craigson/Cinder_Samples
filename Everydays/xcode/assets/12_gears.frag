#define TWO_PI 6.28318530718
#define NUM_CIRCLES 12

uniform vec2 uResolution;
uniform float uTime;

out vec4 oColour;

float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
    return smoothstep(_radius-(_radius*0.01),
                      _radius+(_radius*0.01),
                      dot(dist,dist)*4.0);
}

float gear(vec2 position, float radius, float direction, float offset){
    float rad = length(position) * radius;
    float a = direction * (uTime + offset) + atan(position.y,position.x);
    float f = smoothstep(-.5,1., cos(a*10.))*0.2+0.5;
    return f;
}

float gear(vec2 position, float radius, float direction){
    float rad = length(position) * radius;
    float a = direction * uTime + atan(position.y,position.x);
    float f = smoothstep(-.5,1., cos(a*10.))*0.2+0.5;
    return f;
}


void main(void){
    
    vec2 st = gl_FragCoord.xy/uResolution.xy;

    vec3 color = vec3(0.2);
    
    vec2 pos = vec2(0.5)-st;
    vec2 pos2 = 0.219 + vec2(0.5)-st;
    vec2 pos3 = -0.021 + vec2(0.3)-st;
    vec2 pos4 = vec2(0.33,0.76)-st - 0.03;
    vec2 pos5 = vec2(0.76, 0.33)-st - 0.03;
    vec2 pos6 = vec2(0.2)-st - 0.03;
    
    float r = length(pos)*4.0;
    float r2 = length(pos2) * 4.0;
    float r3 = length(pos3) * 4.0;
    float r4 = length(pos4) * 4.0;
    float r5 = length(pos5) * 4.0;
    float r6 = length(pos6) * 4.0;
    
    float a = uTime + atan(pos.y,pos.x);
    
    
    float f1 = gear( vec2(0.5)-st, .5, 1.);
    float f2 = gear(pos2, .5, -1.);
    float f3 = gear(pos3, .5, -1.);
    float f4 = gear(pos4, .5, -1., 0.5);
    float f5 = gear(pos5, .5, -1., 0.75);
    float f6 = gear(pos6, .5, -1., 0.75);
    
    
    color = vec3( 1.-smoothstep(f1,f1 + 0.02 ,r) );
    color += vec3( 1.-smoothstep(f2,f2+0.02,r2) );
    color += vec3( 1.-smoothstep(f3,f3+0.02,r3) );
    color += vec3( 1.-smoothstep(f4,f4+0.02,r4) );
    color += vec3( 1.-smoothstep(f5,f5+0.02,r5) );
//    color += vec3( 1.-smoothstep(f6,f6+0.01,r6) );
    
    oColour = vec4(color,1.0);
}