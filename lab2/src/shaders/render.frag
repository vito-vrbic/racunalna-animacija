#version 460 core

// ### IN variables.
in float vAge;      // The age of the particle.
in vec2 vUV;        // The UV coordinate in the fragment/pixel.
in vec4 pColorTint; // Tint of the particle.

// ### UNIFORM variables.
uniform sampler2D image;    // The image texture, if it exists.
uniform bool has_image;     // Does the image texture exist.

// ### OUT variables
out vec4 FragColor; // Color of the fragment.

void main()
{
    vec4 image_color = has_image ? texture(image, vUV) : vec4(1.0);

    float alpha = image_color.a * pColorTint.a;

    alpha = max(alpha, 0.0);
    
    FragColor = vec4(image_color.rgb * pColorTint.rgb, alpha);
}