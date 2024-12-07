#version 330 core
layout (location = 0) in vec3 aPos;


out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = vec3(1.0, 0.0,0.0);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}