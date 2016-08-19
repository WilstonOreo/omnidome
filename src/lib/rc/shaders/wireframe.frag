#version 330

uniform vec4 color;
out vec4 fragColor;

//Fragment Shader
void main(void)
{
    fragColor = color;
}
