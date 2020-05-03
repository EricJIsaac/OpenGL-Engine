#version 330 core

layout(location = 0) in vec3 vert_ms;
layout(location = 1) in vec3 norm_ms;
layout(location = 2) in ivec4 bone_ids;
layout(location = 3) in vec4 bone_weights;


out vec3 norm_ws;
out vec3 pos_ws;

uniform mat4 uniform_mvp;
uniform mat4 uniform_bpos[50];

void main(){
     mat4 bone = uniform_bpos[bone_ids[0]] * bone_weights[0];
     bone += uniform_bpos[bone_ids[1]] * bone_weights[1];
     bone += uniform_bpos[bone_ids[2]] * bone_weights[2];
     bone += uniform_bpos[bone_ids[3]] * bone_weights[3];

     gl_Position = uniform_mvp * bone * vec4(vert_ms, 1);
     norm_ws = (uniform_mvp * bone * vec4 (norm_ms, 0)).xyz;
     pos_ws = vec3(gl_Position);
}
