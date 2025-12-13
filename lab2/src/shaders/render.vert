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
};

// The particles SSBO.
layout(std430, binding = 0) buffer Particles 
{
    Particle particles[];
};

// ### UNIFORM variables.
uniform mat4 view;  // Camera view matrix.
uniform mat4 proj;  // Window projection matrix.

// ### OUT variables.
out float vAge;         // Age of the particle (for the color interpolation), will be the same for all four vertices.
out vec2 vUV;           // UV coordinates per vertex (interpolated).
out vec3 pColorTint;    // Tint of the particle.

// Gets the depth of a world position from the camera position.
float get_depth_01(vec3 worldPos)
{
    // Transform into view space
    float viewZ = (view * vec4(worldPos, 1.0)).z;

    // Convert to positive depth
    float depth = -viewZ;

    // Tune these values to your scene
    float nearDist = 1.0;
    float farDist  = 50.0;

    return clamp(
        (depth - nearDist) / (farDist - nearDist),
        0.0,
        1.0
    );
}

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

    // Extract camera right and up vectors from view matrix to be able to turn the quad towards the camera.
    vec3 camera_right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cam_up = vec3(view[0][1], view[1][1], view[2][1]);
    vec3 cam_forward = -vec3(view[0][2], view[1][2], view[2][2]);

    // Scale quad by particle size.
    vec3 offset = (camera_right * aPos.x + cam_up * aPos.y) * p.size;

    // Get the depth of each particle.
    float depth01 = smoothstep(0.0, 1.0, get_depth_01(p.position));
    float depthOffset = mix(-5.0, 5.0, depth01);

    // Transform particle position
    gl_Position = proj * view * vec4(p.position + cam_forward * depthOffset + offset, 1.0);

    vAge = p.age;
    vUV = aUV;
    pColorTint = vec3(1.0, 1.0 - p.age, 0.0);
}