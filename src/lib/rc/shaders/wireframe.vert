#version 330

layout(location = 0)in vec3 vertex;

uniform mat4 matrix;

void main(void)
{
    gl_Position = matrix * vec4(vertex, 1);
}
