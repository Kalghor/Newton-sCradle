#version 450 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec3 inColor;

out gl_PerVertex {
	vec4 gl_Position;
};

out vec3 vertColor;

void main(){
	gl_Position = vec4(inPos, 0.f, 1.f);
	vertColor = inColor;
}