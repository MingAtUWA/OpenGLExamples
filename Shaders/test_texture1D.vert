#version 330

layout (location = 0) in vec3 in_pos;
layout (location = 1) in float in_text;

out float obj_text;

void main()
{
	obj_text = in_text;
	gl_Position = vec4(in_pos, 1.0f);
}
