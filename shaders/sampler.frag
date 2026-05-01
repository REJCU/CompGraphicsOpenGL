#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D tuxTexture;
uniform sampler2D pngTexture;

void main() {
    vec4 tex1 = texture(tuxTexture, TexCoords);
    vec4 tex2 = texture(pngTexture, TexCoords);
    
    // This blends them together based on the second image's transparency
    FragColor = mix(tex1, tex2, tex2.a); 
}
