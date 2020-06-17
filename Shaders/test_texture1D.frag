#version 330

in float obj_text;

out vec4 frag_color;

uniform sampler1D color_map;
 
void main(void)
{
	frag_color = texture(color_map, obj_text);
	//frag_color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
