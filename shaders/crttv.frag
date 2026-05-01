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

vec2 st = gl_FragCoord.xy / u_resolution;
    vec2 uv0 = st * 2.0 - 1.0; 

    // jitter
    uv0 += sin(vec2(u_time)) * 0.1;
    
    float distSquared = dot(uv0, uv0);
    uv0 *= (1.0 + 0.1 * distSquared);


    float rings = fract(length(uv0) * 4.0 - u_time);
    float d = length(cos(abs(uv0) * 3.0));
    float distPattern = sin(d * 5.0 - u_time) * 0.5 + 0.5;

    vec3 rawColor = vec3(distPattern * rings);
    rawColor *= vec3(abs(uv0.x), abs(uv0.y), abs(sin(u_time)));
    

    float threshold = bayer4x4(gl_FragCoord.xy);
    
    vec3 ditheredColor = step(threshold, rawColor * 2.0);
    ditheredColor *= smoothstep(1.5, 0.5, length(uv0));

    float scanline = sin(gl_FragCoord.y * 1.5) * 0.1 + 0.9;
    
    FragColor = vec4(ditheredColor * scanline, 1.0);
}

