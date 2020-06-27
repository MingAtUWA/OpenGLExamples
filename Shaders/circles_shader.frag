#version 330

in vec3 obj_color;

out vec4 frag_color;

void main()
{
	frag_color = vec4(obj_color, 1.0f);
}
