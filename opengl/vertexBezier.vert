#version 460 core

/*
    Shader for rendering a bezier curve
*/

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aVertex;

#define SEGMENTS 100

/* 
    Code for the quadratic bezier curve
    @param t: the time parameter
    @param p0: the first control point
    @param p1: the second control point
    @return the point on the curve at time t
*/
vec3 quadraticBezier(float t, vec3 p0, vec3 p1, vec3 p2){
    vec3 p = mix(mix(p0, p1, t), mix(p1, p2, t), t);
    return p;
}

void main() {
    
}

