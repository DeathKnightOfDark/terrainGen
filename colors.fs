#version 330 core
in vec3 FragPos;
in vec3 Normal;
uniform vec3 ambientColor;
uniform vec3 cameraPos;
uniform vec3 lightPos;

float near = 0.1;
float far = 100.0;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0*near*far)/ (far+near-z*(far-near));
}

void main()
{
    float multiplier = 0.9;
    vec3 innerAmbient = ambientColor * vec3(multiplier);
    vec3 terrainColor = vec3(0.1,0.6,0.1);
    vec3 lightColor = vec3(0.6,0.6,0.6);
   
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff*lightColor * vec3(multiplier);

    float specularStrength = 0.5;

    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir),0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * vec3(multiplier);
    vec3 resultColor = (innerAmbient+diffuse+specular)*terrainColor;
    float depth = LinearizeDepth(gl_FragCoord.z)/far;
    //if (sin(FragPos.y)<0.2) discard;
    gl_FragColor = vec4(resultColor*depth, 1.0);
    //gl_FragColor = vec4(vec3(depth),1.0);
   
} 