#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image1;
uniform sampler2D image2;

void main() {
    vec4 tex1 = texture( image1, TexCoords);
    vec4 tex2 = texture( image2, TexCoords);
    
    // This blends them together based on the second image's transparency
    FragColor = mix(tex1, tex2, 0.5); 
}
