#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 uniform_mvp;

void main(){
     //gl_Position.xyz = vertexPosition_modelspace;
     //gl_Position.w = 1.0f;

     mat4 _mvp = mat4(1.0);
     gl_Position = uniform_mvp * vec4(vertexPosition_modelspace, 1);
}