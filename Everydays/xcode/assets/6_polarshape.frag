uniform vec2 uResolution;
uniform vec2 u_mouse;
uniform float uTime;

out vec4 oColour;

void main(){
    vec2 st = gl_FragCoord.xy/uResolution.xy;
    vec3 color = vec3(0.0);
    
    vec2 pos = vec2(0.5)-st;
    
    float r = length(pos)*2.0;
    float a = uTime + sin(uTime)/4. + atan(pos.y,pos.x);
    
    float f = cos(a*3.);
    // f = abs(cos(a*3.));
    f = abs(cos(a*2.5))*.5+.3;
    // f = abs(cos(a*12.)*sin(a*3.))*.8+.1;
    // f = smoothstep(-.5,1., cos(a*10.))*0.2+0.5;
    
    color = vec3( 1.-smoothstep(f,f+sin(uTime)*5.,r) );
    
    oColour = vec4(color, 1.0);
}