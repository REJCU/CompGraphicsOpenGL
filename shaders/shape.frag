#version 330 core
uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

out vec4 FragColor;


void main(){
    vec2 st = gl_FragCoord.xy/u_resolution; 

    st.x *= u_resolution.x/u_resolution.y;
    vec3 color = vec3(0.0);
    float distance = 0.0;

    st = st *2.-1.;

    distance = length(sin(abs(st)-.3));

    FragColor = vec4(vec3(fract(distance*10.0)),1.0);
}
