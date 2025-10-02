#version 330 core

// Uniformne varijable.
uniform mat4 PERS_MAT;
uniform mat4 VIEW_MAT;
uniform mat4 MODEL_MAT;

// Ulazne varijable.
layout(location = 0) in vec3 aPos;

// Izlazne varijable.
out vec3 FragPos;

void main()
{
    gl_Position = PERS_MAT * VIEW_MAT * MODEL_MAT * vec4(aPos, 1.0);
    FragPos = aPos;
}