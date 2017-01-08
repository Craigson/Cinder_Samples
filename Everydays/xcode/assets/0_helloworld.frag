#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
//uniform vec2 u_mouse;
uniform float u_time;

out vec4 oColour;

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution;
    oColour = vec4(st.x,st.y,0.0,1.0);
}