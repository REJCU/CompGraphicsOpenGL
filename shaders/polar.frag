#version 330 core
uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

out vec4 FragColor;


void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.0);

    vec2 pos = vec2(0.5)-st; 
    float r = length(pos)*2.;
    float a = atan(pos.y,pos.x);

    float f = abs(a*3.);

    color = vec3(1.-smoothstep(f,f+0.02,r));
    FragColor = vec4(color, 1.0);
}
