#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightSource;

void main()
{
//     FragColor = tex;
//     FragColor = vec4(texture(texture1, TexCoord).rgb, texture(texture1, TexCoord).a);
    vec3 lightColor = vec3(1.0,1.0,1.0);
    vec3 objectColor = vec3(0.5,0.5,0.5);
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightSource - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);

//      FragColor = vec4(result, 1.0);
//     FragColor = texture(ourTexture, TexCoord);
//     FragColor = vec4(0.0, 0.8, 0.0, 1.0);

}