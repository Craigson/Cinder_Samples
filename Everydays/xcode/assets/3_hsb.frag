#define PI 3.14159265359

uniform vec2 uResolution;
uniform vec2 u_mouse;
uniform float uTime;

out vec4 oColour;

//  Function from Iñigo Quiles
//  https://www.shadertoy.com/view/MsS3Wc

vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

void main(){
    vec2 st = gl_FragCoord.xy/uResolution;
    vec3 color = vec3(0.0);
    
    // Use polar coordinates instead of cartesian
    vec2 toCenter = vec2(0.5)-st;
    float angle = atan(sin(uTime)*toCenter.y,cos(uTime)*toCenter.x);
    float radius = length(toCenter)*2.0;
    
    // Map the angle (-PI to PI) to the Hue (from 0 to 1)
    // and the Saturation to the radius
    // color = hsb2rgb(vec3((sin(angle*u_time)/TWO_PI)+0.5,radius*4.,1.0));
    color = hsb2rgb(vec3(angle/(2*PI),radius,1.0));
    
    oColour = vec4(color,1.0);
}