#version 460 core

// ### IN variables.
in float vAge;      // The age of the particle.
in vec2 vUV;        // The UV coordinate in the fragment/pixel.
in vec3 pColorTint; // Tint of the particle.

// ### UNIFORM variables.
uniform sampler2D image;    // The image texture, if it exists.
uniform bool has_image;     // Does the image texture exist.

// ### OUT variables
out vec4 FragColor; // Color of the fragment.

void main()
{
    // If the particle is not alive, discard it.
    if(vAge < 0.0) discard;

    // The image color is set to white just in case.
    vec4 image_color = vec4(1.0, 1.0, 1.0, 1.0);

    // Set the image color variable to the color of the pixel interpolated using the UV coordinates.
    if(has_image) image_color = texture(image, vUV);

    // Return the fragment color with the transparency being the image's transparency.
    FragColor = vec4(image_color.rgb * pColorTint, image_color.a);
}