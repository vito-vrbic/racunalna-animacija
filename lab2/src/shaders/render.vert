#version 460 core

layout(location = 0) in vec2 aPos;      // Quad vertex

struct Particle {
    vec3 position;
    float size;
    vec3 velocity;
    float age;
};

layout(std430, binding = 0) buffer Particles {
    Particle particles[];
};

uniform mat4 view;
uniform mat4 proj;

out float vAge;

void main()
{
    Particle p = particles[gl_InstanceID];

    if (p.age < 0.0)
    {
        gl_Position = vec4(0,0,0,1);
        vAge = 0.0;
        return;
    }

    // Extract camera right and up vectors from view matrix
    vec3 camRight = vec3(view[0][0], view[1][0], view[2][0]); // column 0
    vec3 camUp    = vec3(view[0][1], view[1][1], view[2][1]); // column 1

    // Scale quad by particle size
    vec3 offset = (camRight * aPos.x + camUp * aPos.y) * p.size;

    // Transform particle position
    gl_Position = proj * view * vec4(p.position + offset, 1.0);

    vAge = p.age;
}