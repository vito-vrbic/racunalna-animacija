#version 330 core

// Uniformne varijable.
uniform vec4 COLOR;

// Ulazne varijable.
in vec3 FragPos;

// Izlazne varijable.
out vec4 FragColor;

void main()
{    
    FragColor = COLOR;
}
