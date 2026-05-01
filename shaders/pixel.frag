#version 330 core
out vec4 FragColor;

uniform vec2 u_mouse;
uniform vec2 u_resolution;
uniform float u_time;
uniform  sampler2D inputBuffer;


float random (vec2 uv0) {
        return fract(sin(dot(uv0.xy, vec2(12.9898,78.233)))* 43758.5453123);
    }

void main() {

    vec2 uv0 = gl_FragCoord.xy / u_resolution;

    float pixels = 64.0;

    vec2 uvPixel = floor(uv0 * pixels)*pixels;

    vec4 texColor = texture(inputBuffer, uvPixel);
    
    FragColor = texColor;
}

