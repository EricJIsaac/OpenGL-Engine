#version 330 core

layout(location = 0) in vec3 vert_ms;
layout(location = 1) in vec3 norm_ms;

out vec3 norm_ws;

uniform mat4 uniform_mvp;

void main(){
     gl_Position = uniform_mvp * vec4(vert_ms, 1);
     norm_ws = (uniform_mvp * vec4 (norm_ms, 0)).xyz;
}
