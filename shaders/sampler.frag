#version 330 core
out vec4 FragColor;

in vec3 ourColor;   // From vertex shader
in vec2 TexCoord;   // From vertex shader (Location 2)

uniform vec2 u_resolution;
uniform float u_time;

uniform sampler2D image1; // Linked to GL_TEXTURE0
uniform sampler2D image2; // Linked to GL_TEXTURE1

void main()
{
    // Sample both textures
    vec4 tex1 = texture(image1, TexCoord);
    vec4 tex2 = texture(image2, TexCoord);

    // chromatic abberation
    float offset = 0.005;
    float r = texture(image2, vec2(TexCoord.x + offset, TexCoord.y)).r;
    float g = texture(image2, TexCoord).g;
    float b = texture(image2, vec2(TexCoord.x - offset, TexCoord.y)).b;

    // 
    // float movingcoords = TexCoord + vec2(u_time * 0.5, 0.0);

    // Use col2.a if you want to use the PNG's transparency.
    // FragColor = mix(tex1, tex2, tex1.a) * vec4(ourColor, 1.0); 
    
    //inverse the colors 
    // FragColor = vec4(1.0 - tex2.rgb, tex2.a);

    // scrolling 
    // FragColor = texture(tex2, movingcoords);

    // chromatic abberation
    FragColor = vec4(r,g,b, 1.0);
    
    // FragColor = vec2(image2, movingcoords);
}
