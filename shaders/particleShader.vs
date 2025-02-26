#version 330 core
layout (location = 0) in vec3 in_Position;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
   
    
    FragPos = vec3(model*vec4(in_Position.x, in_Position.y, in_Position.z, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
	

	

}