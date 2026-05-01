#version 330 core
out vec4 FragColor;

uniform vec2 u_mouse;
uniform vec2 u_resolution;
uniform float u_time;

float random (vec2 uv0) {
    return fract(sin(dot(uv0.xy, vec2(12.9898,78.233)))* 43758.5453123);
}

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution;

    vec3 color = vec3(uv.x * 0.2);

    float dither = (random(uv)-0.5) * 0.1;

    color += dither;

    FragColor = vec4(color, 1.0);
}
