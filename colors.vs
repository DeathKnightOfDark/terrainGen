#version 330 core
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;


out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;


void main()
{
   
    Normal = in_Normal;
    FragPos = vec3(model*vec4(in_Position.x, in_Position.y, in_Position.z, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
	//gl_Position = projection * view * vec4(vec3(in_Position.x, in_Position.y, in_Position.z), 1.0);
	//gl_Position = vec4(in_Position.x, 0.5, in_Position.z, 1.0);

	

}