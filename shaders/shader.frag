// if i want to change, just go and change the name of the loading function in main.c 
#version 330 core
out vec4 FragColor;

// uniform vec2 u_mouse;
uniform vec2 u_resolution;
uniform float u_time;

#define PI 3.14159265359

float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
	return 1.-smoothstep(_radius-(_radius*0.01),
                         _radius+(_radius*0.01),
                         dot(dist,dist)*4.0);
}


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


void main() {
   
    // Normalize coordinates (0.0 to 1.0)
    vec2 st = gl_FragCoord.xy/u_resolution;
    vec3 color = vec3(0.0);
    
    // st = tile(st, 4.0);
    st = fract(st);
    // Use a matrix to rotate the space 45 degrees
    st = rotate2D(st, sin(u_time)*PI);

    //color += vec3(cross(st,0.4));


    // Draw a square
    // color = vec3((box(st,vec2(1.8),abs(sin(u_time))));
    // color = vec3(box(st,vec2(0.7),0.01));

    // color = vec3(st,0.0);

    // desmos -
    color = vec3(circle(st, sin(abs(cos(sin(u_time))))));

    FragColor = vec4(color, 1.0);


    //FragColor = vec4(color, 1.0);
}
