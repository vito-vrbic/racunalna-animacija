#version 460 core

in float vAge;
out vec4 FragColor;

void main()
{
    if(vAge < 0) discard;
    else FragColor = vec4(1.0, 1.0 - vAge, 0.0, 1.0);
}