#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) in vec4 fragColour;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColour;

void main()
{
	outColour = fragColour;
}
