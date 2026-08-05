#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
//     vec4 tex = texture(texture1, TexCoord);
//
//     if (tex.a < 0.01)
//         discard;
//
//     FragColor = tex;
//     FragColor = vec4(texture(texture1, TexCoord).rgb, texture(texture1, TexCoord).a);
    FragColor = texture(ourTexture, TexCoord);
//     FragColor = vec4(ourColor,1.0f);
}