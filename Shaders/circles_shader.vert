#version 330

// circle buffer
layout (location = 0) in vec2 node_coord;

// inst buffer
layout (location = 1) in vec2 pt_coord;
layout (location = 2) in float pt_radius;
layout (location = 3) in vec3 pt_color;

out vec3 obj_color;

uniform mat4 proj_mat;

void main()
{
	obj_color = pt_color;
    
    vec2 cur_coord = node_coord * pt_radius + pt_coord;
	gl_Position = proj_mat * vec4(cur_coord, 0.0f, 1.0f);
}
