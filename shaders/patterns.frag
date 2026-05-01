#version 330 core
out vec4 FragColor;

uniform vec2 u_mouse;
uniform vec2 u_resolution;
uniform float u_time;

#define PI 3.14159265359

vec2 rotate2D(vec2 _st, float _angle){
    _st -= 0.5;
    _st =  mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle)) * _st;
    _st += 0.5;
    return _st;
}

vec2 tile(vec2 _st, float _zoom){
    _st *= _zoom;
    return fract(_st);
}

float box(vec2 _st, vec2 _size, float _smoothEdges){
    _size = vec2(0.5)-_size*0.5;
    vec2 aa = vec2(_smoothEdges*0.5);
    vec2 uv = smoothstep(_size,_size+aa,_st);
    uv *= smoothstep(_size,_size+aa,vec2(1.0)-_st);
    return uv.x*uv.y;
}

float circle(in vec2 _st, in float _radius, float _smoothness){
    vec2 dist = _st-vec2(0.5);
	return 1.-smoothstep(_radius - _smoothness,
                         _radius + _smoothness,
                         dot(dist,dist)*4.0);
}

float random (vec2 uv0) {
        return fract(sin(dot(uv0.xy, vec2(12.9898,78.233)))* 43758.5453123);
    }

float bayer4x4(vec2 _uv) {
    // Get integer pixel coordinates
    ivec2 p = ivec2(mod(gl_FragCoord.xy, 4.0));
    
    // 4x4 Bayer Matrix values
    float m[16] = float[](
         0.0/16.0,  8.0/16.0,  2.0/16.0, 10.0/16.0,
        12.0/16.0,  4.0/16.0, 14.0/16.0,  6.0/16.0,
         3.0/16.0, 11.0/16.0,  1.0/16.0,  9.0/16.0,
        15.0/16.0,  7.0/16.0, 13.0/16.0,  5.0/16.0
    );
    
    return m[p.x + p.y * 4];
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution;
    vec2 uv0 = st * 2.0 - 1.0; // centers the coords 
    
    // vec3 color = vec3(0.0);

    float distSquared = dot(uv0, uv0);
    uv0 = uv0 * (1.0 + 0.1 * distSquared);

    float bulge = 0.1;
    uv0 *= 1.0 + bulge * dot(uv0, uv0);
    
    vec2 mouse = u_mouse / u_resolution;

    vec3 color = vec3(uv0.x * 0.2);

    // uv0.x += cos(u_time * 0.2);
    // uv0.y += sin(u_time * 0.2);

    uv0 += vec2(cos(u_time*0.2), sin(u_time*0.2));

    vec2 screenUV = uv0;

    uv0 = tile( uv0 ,4.);
    uv0 = rotate2D(uv0 , sin(u_time));

    //float mask = box(uv0, vec2(0.7), 0.02);
    float mask = circle(uv0,0.6, 0.02);
    float rings = fract(length(uv0) * 4.0 - u_time);
    // float pulse = abs(sin(u_time)) * 0.5;
    //float size = 0.5 + sin(u_time) * 0.3;

    // vec3 boxColor = vec3(mouse.x, mouse.y, abs(sin(u_time)));
    vec3 boxColor = vec3(uv0, abs(sin(u_time)));
    vec3 rawColor = boxColor * mask * rings;

    // distance field small number is squares - larger kinda circles 
    float d = length(cos(abs(uv0)* 3.0 + u_time));
    vec3 dist = vec3(fract(sin(d * 5.0)));

    // float noise = random(uv0);
    float dither = (random(uv0)-0.5) * 0.1;
    float threshold = bayer4x4(gl_FragCoord.xy);
    float scanline = sin(gl_FragCoord.y * 1.5) * 0.1 + 0.9;


    vec3 finalColor = boxColor * mask * dist;
    finalColor += dither;
    // vec3 finalColor = boxColor * dist * rnd;
    vec3 ditheredColor = step(threshold, rawColor * 2.0);
    
    FragColor = vec4(ditheredColor * scanline, 1.0);
}
