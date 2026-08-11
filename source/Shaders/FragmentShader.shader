#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
//     FragColor = tex;
//     FragColor = vec4(texture(texture1, TexCoord).rgb, texture(texture1, TexCoord).a);

    vec4 texColor = texture(ourTexture, TexCoord);

            // Discard the fragment entirely if it's transparent
            if (texColor.a < 0.1) {
                discard;
            }
        FragColor = texColor;
//     FragColor = texture(ourTexture, TexCoord);
//     FragColor = vec4(0.0, 0.8, 0.0, 1.0);

}