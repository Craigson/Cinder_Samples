#define TWO_PI 6.28318530718

uniform vec2 uResolution;
uniform float uTime;

out vec4 oColour;

//  Function from Iñigo Quiles
//  https://www.shadertoy.com/view/MsS3Wc
vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     clamp( abs( sin(uTime/2.) ) * 5.0, 0.5, 5.0 )  );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

float parabola( float x, float k )
{
    return pow( 4.0*x*(1.0-x), k );
}



void main(){
    vec2 st = gl_FragCoord.xy/uResolution;
    vec3 color = vec3(0.0);
    
    // Use polar coordinates instead of cartesian
    vec2 toCenter = vec2(0.5)-st;
    float angle = uTime+abs(cos(uTime))+atan(toCenter.y,toCenter.x);
    float radius = length(toCenter)*2.0;
    
    float dist = distance(toCenter, st - vec2(0.5,0.5));
    
    // Map the angle (-PI to PI) to the Hue (from 0 to 1)
    // and the Saturation to the radius
    // color = hsb2rgb(vec3((sin(angle*uTime)/TWO_PI)+0.5,radius*4.,1.0));
    
    color = hsb2rgb(vec3(angle/(TWO_PI/5.),radius,1.0));

    
    oColour = vec4(color,1.0);
}