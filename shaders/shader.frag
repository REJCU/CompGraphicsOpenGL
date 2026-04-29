#version 330 core
out vec4 FragColor;

uniform vec2 u_resolution;
uniform float u_time;

void main() {
    // 1. Normalize coordinates (0.0 to 1.0)
    // We use gl_FragCoord (pixel position) / u_resolution (screen size)
    vec2 st = gl_FragCoord.xy / u_resolution;

    // 2. Create a color based on position and time
    // Red increases left-to-right
    // Green increases bottom-to-top
    // Blue pulses over time
    vec3 color = vec3(st.x, st.y, abs(sin(u_time)));

    FragColor = vec4(color, 1.0);
}
