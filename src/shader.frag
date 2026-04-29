#version 330 core
out vec4 FragColor;

uniform float u_time; // Passed from your C code
uniform vec2 u_resolution; // Window size

void main() {
    // Normalize coordinates (0.0 to 1.0)
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;

    // Create a pulsating color effect using sine waves
    vec3 color = 0.5 + 0.5 * cos(u_time + uv.xyx + vec3(0, 2, 4));

    FragColor = vec4(color, 1.0);
}
