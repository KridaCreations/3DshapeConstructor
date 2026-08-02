#version 330 core

out vec4 FragColor;

in vec3 ourColor;


void main()
{
//     FragColor = texture(ourTexture, TexCoord);
//     FragColor = vec4(TexCoord, 0.0, 1.0);
    FragColor = vec4(ourColor,1.0f);
}