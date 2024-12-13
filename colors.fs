#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

float near = 0.1;
float far = 100.0;

float LineriazeDepth(float depth) 
{
    float z = depth*2.0 - 1.0;
    return (2.0*near*far)/(far+near-z*(far-near));
}


void main()
{
   
    vec3 colorBegin = vec3(0.5, 0.0, 0);
    vec3 colorEnd = vec3(0.0,0.5,0.0);
    vec3 colorDiff = colorEnd - colorBegin;
   

    vec3 fragPointColor = vec3(0.1, 0.1, 0.1)*(FragPos.y*0.6+0.2);
    float depth = LineriazeDepth(gl_FragCoord.z)/far;

    //FragColor = vec4(fragPointColor, 1.0);
    vec3 norm = normalize(Normal);
    FragColor = vec4(norm, 1.0);
} 