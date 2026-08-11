#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightSource;

void main()
{
    vec3 lightColor = vec3(1.0,1.0,1.0);
    vec3 objectColor = vec3(0.5,0.5,0.5);
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightSource - FragPos) * -1;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 newLightSource = vec3(lightSource.x *-1,lightSource.y,lightSource.z*-1);
    vec3 newLightDir = normalize(newLightSource - FragPos) * -1;
    float newDiff = max(dot(norm, newLightDir), 0.0);
    vec3 newDiffuse = newDiff * lightColor;

    vec3 result = (ambient + diffuse + newDiffuse) * objectColor;
    FragColor = vec4(result, 1.0);




}