#ifndef GERSTNER_HLSL_INCLUDED
#define GERSTNER_HLSL_INCLUDED
// Function which finds the world position of a vertex for one Gerstner wave.
void GerstnerPosition_float
(
    float3 world_pos,
    float height,
    float frequency,
    float speed,
    float peak,
    float3 direction,
    float TIME,
    out float3 Position
)
{
    float2 dir = normalize(direction.xz);

    float k = frequency;
    float w = k * speed;

    float phase = k * dot(dir, world_pos.xz) + w * TIME;

    float cosP = cos(phase);
    float sinP = sin(phase);

    // Horizontal displacement (XZ).
    float2 horizontal = peak * height * dir * cosP;

    // Vertical displacement.
    float vertical = height * sinP;

    Position = float3
    (
        world_pos.x + horizontal.x,
        vertical,
        world_pos.z + horizontal.y
    );
}

// Function which finds the normal of a vertex for one Gerstner wave.
void GerstnerNormal_float
(
    float3 world_pos,
    float height,
    float frequency,
    float speed,
    float peak,
    float3 direction,
    float TIME,
    out float3 Normal
)
{
    float2 dir = normalize(direction.xz);

    float k = frequency;
    float w = k * speed;

    float phase = k * dot(dir, world_pos.xz) + w * TIME;

    float cosP = cos(phase);
    float sinP = sin(phase);

    // Partial derivatives (tangent vectors).
    float3 tangentX = float3(1, dir.x * height * k * cosP, 0) - peak * height * k * float3(dir.x * dir.x, 0, dir.x * dir.y) * sinP;
    float3 tangentZ = float3(0, dir.y * height * k * cosP, 1) - peak * height * k * float3(dir.x * dir.y, 0, dir.y * dir.y) * sinP;

    // Normal = cross product of tangent vectors.
    Normal = normalize(cross(tangentZ, tangentX));
}

#endif //GERSTNER_HLSL_INCLUDED
