#version 460 core

// ### IN variables.
layout(location = 0) in vec2 aPos;      // Quad vertex.
layout(location = 1) in vec2 aUV;       // Quad UV coordinates.

struct Particle
{
    vec3 position;
    float size;
    vec3 velocity;
    float age;
    float life_lentgh;
};

// The particles SSBO.
layout(std430, binding = 0) buffer Particles 
{
    Particle particles[];
};

// ### UNIFORM variables.
uniform mat4 view;  // Camera view matrix.
uniform mat4 proj;  // Window projection matrix.
uniform vec3 cam_right;
uniform vec3 cam_up;
uniform vec3 cam_forward;
uniform vec4 start_color;   // Starting color for interpolation.
uniform vec4 end_color;     // End color for interpolation.

// ### OUT variables.
out float vAge;         // Age of the particle (for the color interpolation), will be the same for all four vertices.
out vec2 vUV;           // UV coordinates per vertex (interpolated).
out vec4 pColorTint;    // Tint of the particle.

void main()
{
    // Get the particle to render.
    Particle p = particles[gl_InstanceID];

    // If the particle's age is less than 0, return nullable data.
    if (p.age < 0.0)
    {
        gl_Position = vec4(0,0,0,1);
        vAge = 0.0;
        vUV = vec2(0.0,0.0);
        return;
    }

    // Scale quad by particle size.
    vec3 offset = (cam_right * aPos.x + cam_up * aPos.y) * p.size;

    // Transform particle position
    gl_Position = proj * view * vec4(p.position + offset, 1.0);

    vAge = p.age;
    vUV = aUV;
    pColorTint = mix(start_color, end_color, p.age);
}