#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float textureScale;

out float Height;
out vec2 TexCoord;

void main()
{
    Height = aPos.y;
    // changing this changes the side of the texture, so making constsant bigger basically scales the texture to be bigger
    TexCoord = aPos.xz / textureScale;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
