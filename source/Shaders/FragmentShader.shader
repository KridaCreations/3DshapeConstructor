#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
//     FragColor = tex;
//     FragColor = vec4(texture(texture1, TexCoord).rgb, texture(texture1, TexCoord).a);
    FragColor = texture(ourTexture, TexCoord);
//     FragColor = vec4(0.0, 0.8, 0.0, 1.0);

}